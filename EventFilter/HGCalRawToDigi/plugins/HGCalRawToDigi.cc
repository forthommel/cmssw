#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

#include "EventFilter/HGCalRawToDigi/interface/TestBeamUnpackerAlgo.h"

class HGCalRawToDigi : public edm::global::EDProducer<> {
public:
  explicit HGCalRawToDigi(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void produce(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;
  std::unique_ptr<hgcal::UnpackerAlgo> algo_;
  edm::EDGetTokenT<FEDRawDataCollection> fedDataToken_;
};

HGCalRawToDigi::HGCalRawToDigi(const edm::ParameterSet& iConfig)
    : fedDataToken_(consumes<FEDRawDataCollection>(iConfig.getParameter<edm::InputTag>("rawDataTag"))) {
  produces<HGCalDigiCollection>();
  const auto algo_name = iConfig.getParameter<std::string>("algo");
  if (algo_name == "TestBeam")
    algo_ = std::make_unique<hgcal::TestBeamUnpackerAlgo>(iConfig);
  else
    throw cms::Exception("HGCalRawToDigi") << "Invalid unpacker algorithm: '" << algo_name << "'.";
}

void HGCalRawToDigi::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const {
  auto digis = std::make_unique<HGCalDigiCollection>();

  const auto& raw_data = iEvent.get(fedDataToken_);  // FEDRawDataCollection
  const auto& fed_data = raw_data.FEDData(0);        // FEDRawData
  if (fed_data.size() > 0)
    algo_->run(fed_data, *digis);

  iEvent.put(std::move(digis));
}

void HGCalRawToDigi::fillDescriptions(edm::ConfigurationDescriptions& description) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("algo", "TestBeam");
  desc.add<edm::InputTag>("rawDataTag", edm::InputTag(""));
  description.add("hgcalDigis", desc);
}

DEFINE_FWK_MODULE(HGCalRawToDigi);
