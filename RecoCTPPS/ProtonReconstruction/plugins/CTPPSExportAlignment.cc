/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ValidityInterval.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/RPAlignmentCorrectionsMethods.h"

#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"

#include "RecoCTPPS/ProtonReconstruction/interface/alignment_classes.h"
#include "RecoCTPPS/ProtonReconstruction/interface/fill_info.h"

//----------------------------------------------------------------------------------------------------

class CTPPSExportAlignment : public edm::stream::EDAnalyzer<>
{
  public:
    explicit CTPPSExportAlignment(const edm::ParameterSet&);
    ~CTPPSExportAlignment() {}

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    void doExport(const CTPPSGeometry &geometry) const;

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    std::vector<std::string> inputFiles;
    std::string outputFile;

    bool done;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSExportAlignment::CTPPSExportAlignment(const edm::ParameterSet& iConfig) :
  inputFiles(iConfig.getParameter<vector<string>>("inputFiles")),
  outputFile(iConfig.getParameter<string>("outputFile")),
  done(false)
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::analyze(const edm::Event&, const edm::EventSetup &iSetup)
{
  if (done)
    return;

  ESHandle<CTPPSGeometry> geometry;
  iSetup.get<VeryForwardRealGeometryRecord>().get(geometry);

  doExport(*geometry);

  done = true;
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::doExport(const CTPPSGeometry &geometry) const
{
  // load input
  AlignmentResultsCollection input;
  for (const auto &f : inputFiles)
  {
    FileInPath fip(f);
    if (input.Load(fip.fullPath()) != 0)
      throw cms::Exception("CTPPSExportAlignment") << "Cannot load alignment from file '" << f << "'.";
  }

  // load fill-alignment mapping
  InitFillInfoCollection();

  // export alignment
  RPAlignmentCorrectionsDataSequence alSeq;
  for (const auto &fi : fillInfoCollection)
  {
    printf("* fill %u\n", fi.fillNumber);

    const IOVSyncValue beg(EventID(fi.runMin, 1, 1));
    const IOVSyncValue end(EventID(fi.runMax, EventID::maxLuminosityBlockNumber(), EventID::maxEventNumber()));
    edm::ValidityInterval iov(beg, end);

    const auto &alInput = input[fi.alignmentTag];
    RPAlignmentCorrectionsData alData;
    for (const auto &p : alInput)
    {
      const unsigned int rpDecId = p.first;
      const unsigned int arm = rpDecId / 100;
      const unsigned int station = (rpDecId / 10) % 10;
      const unsigned int rp = rpDecId % 10;

      unsigned int subDetector = CTPPSDetId::sdTrackingStrip;
      if (station == 2)
        subDetector = CTPPSDetId::sdTrackingPixel;
      CTPPSDetId rpId(subDetector, arm, station, rp);

      const AlignmentResult &ar = p.second;

      RPAlignmentCorrectionData rpCorrection(
        ar.sh_x, ar.sh_x_unc,
        -ar.sh_y, ar.sh_y_unc,
        0., 0.,
        ar.rot_x, ar.rot_x_unc,
        ar.rot_y, ar.rot_y_unc,
        -ar.rot_z, ar.rot_z_unc
      );

      alData.setRPCorrection(rpId, rpCorrection);

      // extras for 2016: fill from 4947 to 5288
      if (rpDecId == 2 && fi.fillNumber >= 4947 && fi.fillNumber <= 5288)
      {
        for (unsigned int plane = 0; plane < 10; ++plane)
        {
          TotemRPDetId planeId(arm, station, rp, plane);

          auto c = geometry.getSensorTranslation(planeId);
          printf("  plane = %u, c_x = %.3f, c_y = %.3f, c_z = %.3f\n", plane, c.x(), c.y(), c.z());

          ROOT::Math::RotationZYX R(-ar.rot_z, ar.rot_y, ar.rot_x);
          ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>> cm(c.x(), c.y(), c.z());
          auto de = R * cm - cm;

          RPAlignmentCorrectionData sensorCorrection(
            de.x(), 0., de.y(), 0., de.z(), 0.,
            0., 0., 0., 0., 0., 0.
          );

          alData.setSensorCorrection(planeId, sensorCorrection);
        }
      }
    }

    alSeq.insert(iov, alData);
  }

  // save output
  RPAlignmentCorrectionsMethods::writeToXML(alSeq, outputFile,
    false, false, true, true, true, true);
}

//----------------------------------------------------------------------------------------------------

void CTPPSExportAlignment::fillDescriptions(ConfigurationDescriptions& descriptions)
{
  ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSExportAlignment);
