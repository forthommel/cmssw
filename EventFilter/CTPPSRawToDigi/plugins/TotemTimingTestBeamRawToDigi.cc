/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <memory>
#include <unordered_map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSDigi/interface/TotemTimingDigi.h"

#include "TTree.h"

class TotemTimingTestBeamRawToDigi : public edm::stream::EDProducer<> {
public:
  explicit TotemTimingTestBeamRawToDigi(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;
  TTree* input_tree_;
  std::unordered_map<unsigned short, unsigned short> detid_vs_chid_;
};

TotemTimingTestBeamRawToDigi::TotemTimingTestBeamRawToDigi(const edm::ParameterSet& iConfig) {
  // build the tree Id -> DetId from the python configuration
  for (const auto& id_map : iConfig.getParameter<std::vector<edm::ParameterSet>>("idsMapping"))
    detid_vs_chid_[id_map.getParameter<unsigned int>("treeChId")] =
        TotemTimingDetId(id_map.getParameter<unsigned int>("detId"));
  // specify what will be produced by this EDProducer
  produces<edm::DetSetVector<TotemTimingDigi>>();
}

void TotemTimingTestBeamRawToDigi::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // output collection
  auto digis = std::make_unique<edm::DetSetVector<TotemTimingDigi>>();

  //--- example of an insertion with a TTree-obtained channel Id

  // for (unsigned long long ev_id = 0; ev_id < input_tree_->GetEntriesFast(); ++ev_id) {
  // collection of "fake info" to be filled from TTree read/recasted event info
  unsigned short ch_id = 0;  // <-- most important part to perform channel mapping
  uint8_t hw_id = 0;
  uint64_t fpga_timestamp = 0;
  uint16_t timestamp_a = 0, timestamp_b = 0, cell_info = 0;
  std::vector<uint8_t> samples(64);
  TotemTimingEventInfo evt_info;

  // obtain DetId from lookup table defined in configuration
  const auto detid = TotemTimingDetId(detid_vs_chid_.at(ch_id));

  // add a new DetId-indexed collection ("vector"-like)
  edm::DetSet<TotemTimingDigi> digis_for_detid = digis->find_or_insert(detid);
  // inline addition of a new TotemTimingDigi (see https://github.com/cms-sw/cmssw/blob/master/DataFormats/CTPPSDigi/interface/TotemTimingDigi.h#L21-L27)
  digis_for_detid.emplace_back(hw_id, fpga_timestamp, timestamp_a, timestamp_b, cell_info, samples, evt_info);
  //}

  iEvent.put(std::move(digis));
}

void TotemTimingTestBeamRawToDigi::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;

  // validator for a mapping entry in the python configuration
  edm::ParameterSetDescription idmap_valid;
  idmap_valid.add<unsigned int>("treeChId", 0)->setComment("HW id as retrieved from tree");
  idmap_valid.add<unsigned int>("detId", 0)->setComment("mapped TotemTimingDetId for this channel");

  std::vector<edm::ParameterSet> idmap_default;
  // add three channels mapping as an example (can be done in python configuration directly)
  // TotemTimingDetId(arm, station, rp, plane, channel) (see https://github.com/cms-sw/cmssw/blob/master/DataFormats/CTPPSDetId/interface/TotemTimingDetId.h#L36)

  // tree channel 0 -> plane 0, channel 0
  auto& ch0 = idmap_default.emplace_back();
  ch0.addParameter<unsigned int>("treeChId", 0);
  ch0.addParameter<unsigned int>("detId", TotemTimingDetId(0, 0, 0, 0, 0));
  // tree channel 1 -> plane 0, channel 1
  auto& ch1 = idmap_default.emplace_back();
  ch1.addParameter<unsigned int>("treeChId", 1);
  ch1.addParameter<unsigned int>("detId", TotemTimingDetId(0, 0, 0, 0, 1));
  // tree channel 2 -> plane 1, channel 0
  auto& ch2 = idmap_default.emplace_back();
  ch2.addParameter<unsigned int>("treeChId", 2);
  ch2.addParameter<unsigned int>("detId", TotemTimingDetId(0, 0, 0, 1, 0));

  // add this configuration (and the validator) to generate automatically a python configuration (see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideConfigurationValidationAndHelp)
  desc.addVPSet("idsMapping", idmap_valid, idmap_default);

  descriptions.add("totemTimingTestBeamDigis", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TotemTimingTestBeamRawToDigi);
