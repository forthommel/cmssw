#include "EventFilter/HGCalRawToDigi/interface/TestBeamUnpackerAlgo.h"

#include <cstring>

namespace hgcal {
  TestBeamUnpackerAlgo::TestBeamUnpackerAlgo(const edm::ParameterSet& iConfig)
      : UnpackerAlgo(iConfig),
        idle_pattern_(iConfig.getUntrackedParameter<unsigned int>("idlePattern", 0x555500)),
        header_marker_(iConfig.getUntrackedParameter<unsigned int>("headerMarker", 0x154)) {}

  static uint32_t get_bits(uint32_t word, char start, char length) {
    // this function get [n, n + length] bits of word
    uint32_t temp = (1 << length) - 1;
    return word >> start & temp;
  }

  void TestBeamUnpackerAlgo::run(const FEDRawData& input, HGCalDigiCollection& digis) {
    // first convert 8-bit buffer into 32-bit unsigned integers
    std::vector<unsigned int> input_uint32(input.size() / sizeof(unsigned int));
    memcpy(input_uint32.data(), input.data(), input.size() / sizeof(unsigned int));

    // parse the FED frame into ECON-D format
    ECOND econd;
    for (auto it = input_uint32.begin(); it != input_uint32.end(); ++it) {
      const auto word = *it;
      if (word >> 8 == idle_pattern_)
        continue;  // skip idle parts
      if (word >> 23 == header_marker_) {
        // use payload length to get body info of ECON-D
        econd.payload_length = get_bits(word, 14, 9);
        econd.header = ((uint64_t)word) << 32;
        econd.header = econd.header | (uint64_t)(*(it++));
        for (size_t j = 0; j < econd.payload_length; ++j)
          econd.body[j] = *(it++);
        // convert the ECON-D to ERX
        ERX erx;
        unsigned int index = 0;
        if (!convertECONDtoERX(econd, erx, index))
          break;
        printf("===== header ===== 0x%lx\n", erx.header);
        for (uint8_t j = 0; j < erx.channel_number; j++) {
          if (erx.type[j] == 1)
            printf("%04x\n", erx.body[j]);
          else if (erx.type[j] >= 4)
            printf("%08x\n", erx.body[j]);
          else
            printf("%06x\n", erx.body[j]);
        }
        break;
      } else
        throw cms::Exception("TestBeamUnpackerAlgo")
            << "Reading the word '0x" << std::hex << word << std::dec << "' that is neither idle nor event header!";
    }
  }

  bool TestBeamUnpackerAlgo::convertECONDtoERX(const ECOND& econd, ERX& erx, unsigned int& index) const {
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
  }
}  // namespace hgcal
