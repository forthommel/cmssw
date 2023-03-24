/****************************************************************************
 *
 * This is a part of HGCal offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/DataRecord/interface/HGCalLabTestConditionsRcd.h"
#include "CondFormats/HGCalObjects/interface/HGCalLabTestConditions.h"

class HGCalLabTestConditionsAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit HGCalLabTestConditionsAnalyzer(const edm::ParameterSet& iConfig)
      : tokenConds_(esConsumes<HGCalLabTestConditions, HGCalLabTestConditionsRcd>(
            edm::ESInputTag(iConfig.getParameter<std::string>("label")))) {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<std::string>("label", {});
    descriptions.addWithDefaultLabel(desc);
  }

private:
  void analyze(const edm::Event&, const edm::EventSetup& iSetup) override {
    // get timing calibration parameters
    if (calibWatcher_.check(iSetup))
      edm::LogInfo("HGCalLabTestConditionsAnalyzer") << "Conditions retrieved:\n" << iSetup.getData(tokenConds_);
  }

  edm::ESWatcher<HGCalLabTestConditionsRcd> calibWatcher_;
  edm::ESGetToken<HGCalLabTestConditions, HGCalLabTestConditionsRcd> tokenConds_;
};

DEFINE_FWK_MODULE(HGCalLabTestConditionsAnalyzer);
