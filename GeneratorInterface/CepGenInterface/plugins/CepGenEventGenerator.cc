// CepGen-CMSSW interfacing module
//   2022, Laurent Forthomme

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

class test : public edm::stream::EDProducer<> {
public:
  explicit test(const edm::ParameterSet&);
  ~test() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void endRun(const edm::Run&, const edm::EventSetup&) override;
};

CepGenEventGenerator::CepGenEventGenerator(const edm::ParameterSet& iConfig) {
  //produces<ExampleData2>();
}

void CepGenEventGenerator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  //iEvent.put(std::make_unique<ExampleData2>(in));
}

void CepGenEventGenerator::beginRun(const edm::Run&, const edm::EventSetup&) {
  // dummy
}

void CepGenEventGenerator::endRun(const edm::Run&, const edm::EventSetup&) {
  // dummy
}

void CepGenEventGenerator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CepGenEventGenerator);
