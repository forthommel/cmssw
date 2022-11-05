#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

#include "EventFilter/HGCalRawToDigi/interface/RawDataPackingTools.h"

#include "CLHEP/Random/RandFlat.h"
#include "TChain.h"

class HGCalFEDEmulator : public edm::stream::EDProducer<> {
public:
  explicit HGCalFEDEmulator(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;

  const std::vector<std::string> input_files_;
  const unsigned int num_channels_;
  const unsigned int header_marker_;
  const unsigned int idle_marker_;
  const unsigned int fed_id_;
  const double chan_surv_prob_;

  std::unique_ptr<TChain> chain_;

  struct TreeEvent {
    int event, chip, half, channel, adc, adcm, toa, tot, totflag, bxcounter, eventcounter, orbitcounter;
  };
  typedef std::tuple<uint32_t, uint32_t, uint32_t> Event_t;
  typedef std::tuple<uint8_t, uint8_t> ERx_t;
  struct ERxData_t {
    std::vector<uint16_t> adc, adcm, toa, tot;
    std::vector<uint8_t> tctp;
    uint32_t cm0, cm1;
  };
  typedef std::map<Event_t, std::map<ERx_t, ERxData_t>> ECONDInputs_t;
  ECONDInputs_t data_;
  ECONDInputs_t::const_iterator it_data_;

  edm::Service<edm::RandomNumberGenerator> rng_;
  edm::EDPutTokenT<FEDRawDataCollection> fedRawToken_;
};

HGCalFEDEmulator::HGCalFEDEmulator(const edm::ParameterSet& iConfig)
    : input_files_(iConfig.getParameter<std::vector<std::string>>("inputs")),
      num_channels_(iConfig.getParameter<unsigned int>("numChannels")),
      header_marker_(iConfig.getParameter<unsigned int>("headerMarker")),
      idle_marker_(iConfig.getParameter<unsigned int>("idleMarker")),
      fed_id_(iConfig.getParameter<unsigned int>("fedId")),
      chan_surv_prob_(iConfig.getParameter<double>("channelSurvProb")),
      chain_(new TChain(iConfig.getParameter<std::string>("treeName").data())) {
  fedRawToken_ = produces<FEDRawDataCollection>();
  if (!rng_.isAvailable())
    throw cms::Exception("HGCalFEDEmulator")
        << "The HGCalFEDEmulator module requires the RandomNumberGeneratorService,\n"
           "which appears to be absent. Please add that service to your configuration\n"
           "or remove the modules that require it.";
}

void HGCalFEDEmulator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  if (it_data_ == data_.end())
    throw cms::Exception("HGCalFEDEmulator") << "Insufficient number of events were retrieved from input tree to "
                                                "proceed with the generation of emulated events.";
  FEDRawDataCollection raw_data;
  auto* rng_engine = &rng_->getEngine(iEvent.streamID());
  std::vector<uint32_t> econ_event;
  for (const auto& jt : it_data_->second) {
    std::vector<bool> chmap(num_channels_, true);
    for (size_t i = 0; i < chmap.size(); i++)
      chmap[i] = CLHEP::RandFlat::shoot(rng_engine) < chan_surv_prob_;

    auto erxData = hgcal::econd::eRxSubPacketHeader(0, 0, 0, jt.second.cm0, jt.second.cm1, chmap);
    for (size_t i = 0; i < num_channels_; i++) {
      if (!chmap[i])
        continue;
      uint8_t msb(32);
      auto chData = hgcal::econd::addChannelData(msb,
                                                 jt.second.tctp[i],
                                                 jt.second.adc[i],
                                                 jt.second.tot[i],
                                                 jt.second.adcm[i],
                                                 jt.second.toa[i],
                                                 true,
                                                 true,
                                                 true,
                                                 true);
      erxData.insert(erxData.end(), chData.begin(), chData.end());
    }

    econ_event.insert(econ_event.end(), erxData.begin(), erxData.end());
  }
  auto econdH = hgcal::econd::eventPacketHeader(header_marker_,
                                                econ_event.size() + 1,
                                                true,
                                                false,
                                                0,
                                                0,
                                                false,
                                                false,
                                                0,
                                                std::get<0>(it_data_->first),
                                                std::get<1>(it_data_->first),
                                                std::get<2>(it_data_->first),
                                                false,
                                                0,
                                                0);
  econ_event.insert(econ_event.begin(), econdH.begin(), econdH.end());
  econ_event.push_back(0);  //CRC is fake
  econ_event.push_back(idle_marker_);

  size_t event_size = econ_event.size() * sizeof(econ_event.at(0));

  // fill the output FED raw data collection
  auto& fed_data = raw_data.FEDData(fed_id_);

  fed_data.resize(event_size);
  memcpy(fed_data.data(), econ_event.data(), event_size);

  iEvent.emplace(fedRawToken_, std::move(raw_data));
  ++it_data_;
}

void HGCalFEDEmulator::beginStream(edm::StreamID) {
  for (const auto& input : input_files_)
    chain_->Add(input.data());

  TreeEvent event;
  chain_->SetBranchAddress("event", &event.event);
  chain_->SetBranchAddress("chip", &event.chip);
  chain_->SetBranchAddress("half", &event.half);
  chain_->SetBranchAddress("channel", &event.channel);
  chain_->SetBranchAddress("adc", &event.adc);
  chain_->SetBranchAddress("adcm", &event.adcm);
  chain_->SetBranchAddress("toa", &event.toa);
  chain_->SetBranchAddress("tot", &event.tot);
  chain_->SetBranchAddress("totflag", &event.totflag);
  chain_->SetBranchAddress("bxcounter", &event.bxcounter);
  chain_->SetBranchAddress("eventcounter", &event.eventcounter);
  chain_->SetBranchAddress("orbitcounter", &event.orbitcounter);

  for (long long i = 0; i < chain_->GetEntries(); i++) {
    chain_->GetEntry(i);
    // check if event already exists
    Event_t key((uint32_t)event.eventcounter, (uint32_t)event.bxcounter, (uint32_t)event.orbitcounter);
    if (data_.count(key) == 0)
      data_[key] = std::map<ERx_t, ERxData_t>();
    // check if chip already exists
    ERx_t erxKey((uint8_t)event.chip, (uint8_t)event.half);
    if (data_[key].count(erxKey) == 0)
      data_[key][erxKey] = ERxData_t();
    // add channel data
    if (event.channel == (int)num_channels_)
      data_[key][erxKey].cm0 = event.adc;
    else if (event.channel == (int)num_channels_ + 1)
      data_[key][erxKey].cm1 = event.adc;
    else {
      data_[key][erxKey].tctp.push_back(event.totflag ? 3 : 0);
      data_[key][erxKey].adc.push_back(event.adc);
      data_[key][erxKey].tot.push_back(event.tot);
      data_[key][erxKey].adcm.push_back(event.adcm);
      data_[key][erxKey].toa.push_back(event.toa);
    }
  }
  it_data_ = data_.begin();
}

void HGCalFEDEmulator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("treeName", "unpacker_data/hgcroc");
  desc.add<std::vector<std::string>>("inputs", {});
  desc.add<unsigned int>("numChannels", 37);
  desc.add<unsigned int>("headerMarker", (0xaa000000 >> 23) & 0x1ff);
  desc.add<unsigned int>("idleMarker", 0x55550000);
  desc.add<unsigned int>("fedId", 0);
  desc.add<double>("channelSurvProb", 999.);
  descriptions.add("hgcalEmulatedFEDRawData", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HGCalFEDEmulator);
