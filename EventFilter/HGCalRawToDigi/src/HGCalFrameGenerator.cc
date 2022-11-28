#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/CRC16.h"

#include "EventFilter/HGCalRawToDigi/interface/HGCalFrameGenerator.h"
#include "EventFilter/HGCalRawToDigi/interface/RawDataPackingTools.h"

#include "CLHEP/Random/RandFlat.h"

namespace hgcal {
  HGCalFrameGenerator::HGCalFrameGenerator(const edm::ParameterSet& iConfig)
      : chan_surv_prob_(iConfig.getParameter<double>("channelSurv")),
        enabled_channels_(iConfig.getParameter<std::vector<unsigned int>>("enabledChannels")),
        header_marker_(iConfig.getParameter<unsigned int>("headerMarker")),
        num_channels_(iConfig.getParameter<unsigned int>("numChannels")),
        bitO_error_prob_(iConfig.getParameter<double>("bitOError")),
        bitB_error_prob_(iConfig.getParameter<double>("bitBError")),
        bitE_error_prob_(iConfig.getParameter<double>("bitEError")),
        bitT_error_prob_(iConfig.getParameter<double>("bitTError")),
        bitH_error_prob_(iConfig.getParameter<double>("bitHError")),
        bitS_error_prob_(iConfig.getParameter<double>("bitSError")) {}

  edm::ParameterSetDescription HGCalFrameGenerator::description() {
    edm::ParameterSetDescription desc;
    desc.add<double>("channelSurv", 1.);
    desc.add<std::vector<unsigned int>>("enabledChannels", {})->setComment("list of channels to be enabled in readout");
    desc.add<unsigned int>("headerMarker", 0x154)->setComment("9b programmable pattern; default is '0xAA' + '0b0'");
    desc.add<unsigned int>("numChannels", 37)->setComment("number of channels managed in ECON-D");
    desc.add<double>("bitOError", 0.);
    desc.add<double>("bitBError", 0.);
    desc.add<double>("bitEError", 0.);
    desc.add<double>("bitTError", 0.);
    desc.add<double>("bitHError", 0.);
    desc.add<double>("bitSError", 0.);
    return desc;
  }

  void HGCalFrameGenerator::setRandomEngine(CLHEP::HepRandomEngine& rng) { rng_ = &rng; }

  HGCalFrameGenerator::HeaderBits HGCalFrameGenerator::generateStatusBits() const {
    HeaderBits header_bits;
    // first sample on header status bits
    header_bits.bitO = CLHEP::RandFlat::shoot(rng_) >= bitO_error_prob_;
    header_bits.bitB = CLHEP::RandFlat::shoot(rng_) >= bitB_error_prob_;
    header_bits.bitE = CLHEP::RandFlat::shoot(rng_) >= bitE_error_prob_;
    header_bits.bitT = CLHEP::RandFlat::shoot(rng_) >= bitT_error_prob_;
    header_bits.bitH = CLHEP::RandFlat::shoot(rng_) >= bitH_error_prob_;
    header_bits.bitS = CLHEP::RandFlat::shoot(rng_) >= bitS_error_prob_;
    return header_bits;
  }

  std::vector<bool> HGCalFrameGenerator::generateEnabledChannels(uint64_t& ch_en) const {
    std::vector<bool> chmap(num_channels_, false);
    ch_en = 0ull;  // reset the list of channels enabled
    for (size_t i = 0; i < chmap.size(); i++) {
      // randomly choosing the channels to be shot at
      chmap[i] = (enabled_channels_.empty() ||
                  (std::find(enabled_channels_.begin(), enabled_channels_.end(), i) != enabled_channels_.end())) &&
                 CLHEP::RandFlat::shoot(rng_) <= chan_surv_prob_;
      ch_en += (chmap[i] << i);
    }
    ch_en &= ((1 << (num_channels_ + 1)) - 1);  // mask only (num_channels_) LSBs
    return chmap;
  }

  std::vector<uint32_t> HGCalFrameGenerator::generateERxData(const econd::ERxEvent& event,
                                                             std::vector<uint64_t>& enabled_channels) const {
    enabled_channels.clear();  // reset the list of channels enabled

    std::vector<uint32_t> erxData;
    for (const auto& jt : event) {
      uint64_t ch_en;  // list of channels enabled
      auto chmap = generateEnabledChannels(ch_en);
      enabled_channels.emplace_back(ch_en);

      auto erxHeader = econd::eRxSubPacketHeader(0, 0, false, jt.second.cm0, jt.second.cm1, chmap);
      erxData.insert(erxData.end(), erxHeader.begin(), erxHeader.end());
      for (size_t i = 0; i < num_channels_; i++) {
        if (!chmap.at(i))
          continue;
        uint8_t msb = 32;
        auto chData = econd::addChannelData(msb,
                                            jt.second.tctp.at(i),
                                            jt.second.adc.at(i),
                                            jt.second.tot.at(i),
                                            jt.second.adcm.at(i),
                                            jt.second.toa.at(i),
                                            true,
                                            true,
                                            true,
                                            true);
        erxData.insert(erxData.end(), chData.begin(), chData.end());
      }
    }
    return erxData;
  }

  std::vector<uint32_t> HGCalFrameGenerator::produceECONEvent(const econd::ECONDEvent& event) const {
    std::vector<uint64_t> enabled_channels;
    auto header_bits = generateStatusBits();
    auto slink_event = generateERxData(event.second, enabled_channels);
    LogDebug("HGCalFrameGenerator") << slink_event.size() << " word(s) of eRx payloads inserted.";

    // as ECON-D event content was just created, only prepend packet header at
    // this stage
    auto econdH = hgcal::econd::eventPacketHeader(
        header_marker_,
        slink_event.size() + 1,
        true,
        false,
        // HGCROC Event reco status across all active eRxE-B-O:
        // FIXME check endianness of these two numbers
        (header_bits.bitH << 1) | header_bits.bitT,                            // HDR/TRL numbers
        (header_bits.bitE << 2) | (header_bits.bitB << 1) | header_bits.bitO,  // Event/BX/Orbit numbers
        false,
        false,
        0,
        std::get<0>(event.first),
        std::get<1>(event.first),
        std::get<2>(event.first),
        header_bits.bitS,  // OR of "Stat" bits for all active eRx
        0,
        0);
    slink_event.insert(slink_event.begin(), econdH.begin(), econdH.end());
    LogDebug("HGCalFrameGenerator") << econdH.size()
                                    << " word(s) of event packet header prepend. New size of ECON frame: "
                                    << slink_event.size();

    slink_event.push_back(computeCRC(econdH));

    return slink_event;
  }

  uint8_t HGCalFrameGenerator::computeCRC(const std::vector<uint32_t>& event_header) const {
    uint8_t crc = 0;  //FIXME 8-bit Bluetooth CRC
    return crc;
  }
}  // namespace hgcal
