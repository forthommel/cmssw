/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 * Based on work by:
 *   Morteza Pourkazemi
 *   Seyed Mohsen Etesami
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "SimPPS/PPSTimingDigiProducer/interface/HPTDCErrorsGenerator.h"

#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"

#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Units/PhysicalConstants.h>

class PPSDiamondDigiProducer : public edm::stream::EDProducer<> {
public:
  explicit PPSDiamondDigiProducer(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  const edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geometryToken_;
  const edm::EDGetTokenT<CrossingFrame<PSimHit> > crossingFrameToken_;
  const double base_time_offset_, time_to_digi_time_;
  const double failed_packing_prob_;

  HPTDCErrorsGenerator hptdc_err_gen_;

  edm::EDPutTokenT<edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiToken_;

  std::unordered_map<unsigned int, double> time_offsets_;
};

PPSDiamondDigiProducer::PPSDiamondDigiProducer(const edm::ParameterSet& iConfig)
    : geometryToken_(esConsumes()),
      crossingFrameToken_(consumes<CrossingFrame<PSimHit> >(iConfig.getParameter<edm::InputTag>("inputs"))),
      base_time_offset_(iConfig.getParameter<double>("baseTimeOffset")),
      time_to_digi_time_(iConfig.getParameter<double>("timeToDigiTime")),
      failed_packing_prob_(iConfig.getParameter<double>("packingFailureProb")),
      hptdc_err_gen_(iConfig.getParameter<edm::ParameterSet>("hptdcErrors")),
      diamondDigiToken_(produces<edm::DetSetVector<CTPPSDiamondDigi> >()) {
  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable())
    throw cms::Exception("Configuration")
        << "This class requires the RandomNumberGeneratorService\n"
           "which is not present in the configuration file.  You must add the service\n"
           "in the configuration file or remove the modules that require it.";
}

void PPSDiamondDigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Service<edm::RandomNumberGenerator> rng;
  // at this stage, we already checked the service was part of the process
  auto* rnd = &rng->getEngine(iEvent.streamID());

  // first "massage" the input collection to classify by diamond channel
  std::unordered_map<unsigned int, std::vector<PSimHit> > simhits;
  MixCollection<PSimHit> allHits{iEvent.getHandle(crossingFrameToken_).product()};
  for (const auto& simhit : allHits) {
    simhits[simhit.detUnitId()].emplace_back(simhit);
    if (time_offsets_.count(simhit.detUnitId()) == 0)
      time_offsets_[simhit.detUnitId()] =
          fabs(iSetup.getData(geometryToken_).sensorTranslation(simhit.detUnitId()).z()) * CLHEP::mm / CLHEP::m *
          base_time_offset_;
  }

  // then iterate on all channels
  edm::DetSetVector<CTPPSDiamondDigi> digis;  // output collection
  for (const auto& rh_vs_detid : simhits) {
    auto& ds = digis.find_or_insert(CTPPSDiamondDetId(rh_vs_detid.first));
    const auto time_offset = time_offsets_.at(rh_vs_detid.first);
    const bool multi_hits = rh_vs_detid.second.size() > 1;
    const unsigned int thr_voltage = 0;              //FIXME
    for (const auto& simhit : rh_vs_detid.second) {  //FIXME use one single simhit? and which one?
      if (CLHEP::RandFlat::shoot(rnd) < failed_packing_prob_)
        continue;  // packing failed ; discard sim-hit
      const auto tof = (simhit.timeOfFlight() * CLHEP::ns / CLHEP::s) - time_offset;
      unsigned int leading_edge = tof * time_to_digi_time_;  // in HPTDC bins
      unsigned int tot = 0. * time_to_digi_time_;            //FIXME in HPTDC bins
      unsigned int trailing_edge = leading_edge + tot;       // in HPTDC bins
      auto hptdc_err = hptdc_err_gen_.generate(rnd);         // no kinematics-dependence so far
      ds.emplace_back(leading_edge, trailing_edge, thr_voltage, multi_hits, hptdc_err.errorFlag());
    }
  }
  iEvent.emplace(diamondDigiToken_, std::move(digis));  // store output collection to event
}

void PPSDiamondDigiProducer::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::endStream() {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("inputs", edm::InputTag("mix", "g4SimHitsCTPPSTimingHits"));
  desc.add<double>("baseTimeOffset", CLHEP::m / CLHEP::s / CLHEP::c_light)
      ->setComment("fixed offset for the time of arrival computation");
  desc.add<double>("timeToDigiTime", 1024 / 25.e-9)
      ->setComment("conversion factor between hit time of arrival (in seconds) and HPTDC bin size");
  desc.add<double>("packingFailureProb", 0.01)->setComment("probability for the FW to pack the frame wrongly");

  auto hptdc_err_desc = HPTDCErrorsGenerator::description();  // individual probabilities of all HPTDC errors
  desc.add<edm::ParameterSetDescription>("hptdcErrors", hptdc_err_desc);

  descriptions.add("PPSDiamondDigitizer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PPSDiamondDigiProducer);
