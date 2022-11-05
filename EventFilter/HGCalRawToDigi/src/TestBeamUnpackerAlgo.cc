#include "EventFilter/HGCalRawToDigi/interface/TestBeamUnpackerAlgo.h"
#include "EventFilter/HGCalRawToDigi/interface/RawDataUnpackingTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <bitset>
#include <cstring>

namespace hgcal {
  TestBeamUnpackerAlgo::TestBeamUnpackerAlgo(const edm::ParameterSet& iConfig)
      : UnpackerAlgo(iConfig),
        idle_pattern_(iConfig.getUntrackedParameter<unsigned int>("idlePattern", 0x555500)),
        header_marker_(iConfig.getUntrackedParameter<unsigned int>("headerMarker", 0x154)) {}

  static std::string read_poi(const std::vector<bool>& poi) {
    std::ostringstream log;
    std::vector<unsigned short> in, out;
    for (size_t i = 0; i < poi.size(); ++i) {
      if (poi.at(i))
        in.emplace_back(i);
      else
        out.emplace_back(i);
    }
    log << "channels: IN:";
    if (in.empty())
      log << " none";
    else
      for (const auto& ch : in)
        log << " " << ch;
    log << ",\n"
        << "         OUT:";
    if (out.empty())
      log << " none";
    else
      for (const auto& ch : out)
        log << " " << ch;
    return log.str();
  }

  void TestBeamUnpackerAlgo::run(const FEDRawData& input, HGCalDigiCollection& digis) {
    // first convert 8-bit buffer into 32-bit unsigned integers
    std::vector<unsigned int> input_uint32(input.size() / sizeof(unsigned int));
    memcpy(input_uint32.data(), input.data(), input.size() / sizeof(unsigned int));

    // parse the FED frame into ECON-D format
    for (auto it = input_uint32.begin(); it != input_uint32.end(); ++it) {
      if (*it >> 8 == idle_pattern_)
        continue;  // skip idle parts
      if (((*it >> ECOND_FRAME::HEADER_POS) & ECOND_FRAME::HEADER_MASK) != header_marker_)
        continue;  // find the header

      auto event_header = econd::eventPacketHeader({*it, *(it + 1)});
      edm::LogPrint("TestBeamUnpackerAlgo") << "ECON-D content: "
                                            << "payload: " << std::dec << event_header.payload << ", "
                                            << "passthrough mode: " << event_header.bitP << ", "
                                            << "L1A: 0x" << std::hex << event_header.l1a << ", orbit # 0x"
                                            << event_header.orb << ", BX # 0x" << event_header.bx << std::dec;

      it += 2;  // skip the event packet header words

      auto erx_header = econd::eRxSubPacketHeader({*it, *(it + 1)});
      edm::LogPrint("TestBeamUnpackerAlgo").log([&](auto& log) {
        log << "eRx subpacket: common mode words: 0x" << std::hex << erx_header.cm0 << " - 0x" << erx_header.cm1
            << std::dec << "\n"
            << read_poi(erx_header.chmap);
      });

      it += 2;  // skip the eRx subpacket header words

      for (size_t i = 0; i < event_header.payload / 8; ++i) {  // unpack all channels
        it += 8;
      }

      //else throw cms::Exception("TestBeamUnpackerAlgo")
      //    << "Reading the word '0x" << std::hex << word << std::dec << "' that is neither idle nor event header!";
    }
  }

  /*bool TestBeamUnpackerAlgo::convertECONDtoERX(const ECOND& econd, ERX& erx, unsigned int& index) const {
    static const std::vector<uint8_t> map_code2type = {0, 1, 2, 3, 4, 4, 4, 4, 6, 6, 6, 6, 5, 5, 5, 5};
    static const std::vector<uint8_t> map_code2length = {
        24, 16, 24, 24, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};

    while (true) {
      if ((int)index == econd.payload_length - 1)
        return false;
      if (econd.body.at(index) >> 25 & 1) {  // empty
        erx.header = (uint64_t)econd.body.at(index) << 32;
        erx.channel_number = 0;
        index++;
        return true;
      }
      erx.header = ((uint64_t)econd.body.at(index) << 32) | ((uint64_t)econd.body.at(index + 1));
      index += 2;
      erx.channel_number = 0;
      uint16_t bit_counter = 0;
      for (uint8_t i = 0; i < ERX_MAX; i++) {  // regular
        if (!((erx.header >> i) & 1))
          continue;  // do not unpack channel if marked disabled in header
        uint16_t temp_index = bit_counter / 32 + index;
        uint8_t temp_bit = bit_counter % 32;
        uint32_t temp_word = temp_bit == 0 ? econd.body.at(temp_index)
                                           : (econd.body.at(temp_index) << temp_bit) |
                                                 (econd.body.at(temp_index + 1) >> (32 - temp_bit));
        uint8_t code = temp_word >> 28, length = map_code2length.at(code);
        erx.channels[erx.channel_number] = i;
        erx.type[erx.channel_number] = map_code2type.at(code);
        erx.body[erx.channel_number] = temp_word >> (32 - length);
        erx.channel_number++;
        bit_counter += length;
      }
      index += bit_counter / 32;
      if (bit_counter % 32 != 0)
        index += 1;
      return true;
    }
  }*/
}  // namespace hgcal
