// -*- C++ -*-
//
// Package:    CalibPPS/TimingCalibration
// Class:      PPSTimingCalibrationAnalyser
//
/**\class PPSTimingCalibrationAnalyser PPSTimingCalibrationAnalyser.cc CalibPPS/TimingCalibration/test/PPSTimingCalibrationAnalyser.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Sun, 23 Oct 2022 08:28:28 GMT
//
//

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "CondFormats/PPSObjects/interface/PPSTimingCalibration.h"
#include "CondFormats/DataRecord/interface/PPSTimingCalibrationRcd.h"

class PPSTimingCalibrationAnalyser : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit PPSTimingCalibrationAnalyser(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void book(const CTPPSGeometry&, const PPSTimingCalibration&) const;

  bool initialised_{false};
  edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geometryToken_;
  edm::ESGetToken<PPSTimingCalibration, PPSTimingCalibrationRcd> timingCalibrationToken_;
};

PPSTimingCalibrationAnalyser::PPSTimingCalibrationAnalyser(const edm::ParameterSet& iConfig)
    : geometryToken_(esConsumes()),
      timingCalibrationToken_(esConsumes(edm::ESInputTag{"", iConfig.getParameter<std::string>("label")})) {
  usesResource(TFileService::kSharedResource);
}

void PPSTimingCalibrationAnalyser::analyze(const edm::Event&, const edm::EventSetup& iSetup) {
  const auto& geom = iSetup.getData(geometryToken_);
  const auto& calib = iSetup.getData(timingCalibrationToken_);
  if (!initialised_) {
    book(geom, calib);
    initialised_ = true;
  }
}

void PPSTimingCalibrationAnalyser::book(const CTPPSGeometry& geom, const PPSTimingCalibration& calib) const {
  edm::Service<TFileService> fs;
  for (size_t arm = 1; arm <= 2; ++arm) {
    TFileDirectory arm_dir = fs->mkdir("arm_" + std::to_string(arm));
    for (const auto& st : geom.stationsInArm(arm)) {
      std::cout << arm << ":" << st << std::endl;
    }
  }
}

void PPSTimingCalibrationAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("label", "")->setComment("Label to the calibration tag to analyse");
  descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PPSTimingCalibrationAnalyser);
