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
    memcpy(&input_uint32[0], input.data(), input.size() / sizeof(unsigned int));

    ECOND econd;
    for (auto it = input_uint32.begin(); it != input_uint32.end(); ++it) {
      const auto word = *it;
      if (word >> 8 == idle_pattern_)
        continue;  // skip idle parts
      if (word >> 23 == header_marker_) {
        // use payload length to get body info of ECOND
        econd.payload_length = get_bits(word, 14, 9);
        econd.header = ((uint64_t)word) << 32;
        econd.header = econd.header | (uint64_t)(*(it++));
        for (size_t j = 0; j < econd.payload_length; ++j)
          econd.body[j] = *(it++);
        return;
      } else
        throw cms::Exception("TestBeamUnpackerAlgo") << "Reading a word that is neither idle nor event header!";
    }
  }
}  // namespace hgcal
