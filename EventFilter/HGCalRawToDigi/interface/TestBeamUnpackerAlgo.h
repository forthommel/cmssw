#ifndef EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h
#define EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h

#include "EventFilter/HGCalRawToDigi/interface/HGCalUnpackerAlgo.h"
#include <array>

namespace hgcal {
  class TestBeamUnpackerAlgo final : public UnpackerAlgo {
  public:
    explicit TestBeamUnpackerAlgo(const edm::ParameterSet&);

    void run(const FEDRawData&, HGCalDigiCollection&) override;

  private:
    static constexpr size_t PAYLOAD_LENGTH_MAX = 469;  ///< maximum payload length in a ECON-D
    static constexpr size_t ERX_MAX = 37;              ///< maximum number of eRx channels

    struct ECOND {
      uint64_t header{0};
      std::array<uint32_t, PAYLOAD_LENGTH_MAX> body;
      uint16_t payload_length{0};
    };
    struct ERX {
      uint64_t header{0};
      std::array<uint8_t, ERX_MAX> channels;
      std::array<uint8_t, ERX_MAX> type;
      std::array<uint32_t, ERX_MAX> body;
      uint8_t channel_number{0};
    };

    bool convertECONDtoERX(const ECOND&, ERX&, unsigned int&) const;

    const unsigned int idle_pattern_;
    const unsigned int header_marker_;
  };
}  // namespace hgcal

#endif
