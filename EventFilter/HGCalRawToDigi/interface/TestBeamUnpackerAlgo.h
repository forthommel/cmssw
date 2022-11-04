#ifndef EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h
#define EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h

#include "EventFilter/HGCalRawToDigi/interface/HGCalUnpackerAlgo.h"

namespace hgcal {
  class TestBeamUnpackerAlgo final : public UnpackerAlgo {
  public:
    explicit TestBeamUnpackerAlgo(const edm::ParameterSet&);

    void run(const FEDRawData&, HGCalDigiCollection&) override;

  private:
    static constexpr size_t PAYLOAD_LENGTH_MAX = 469;  ///< maximum payload length in a ECON-D
    static constexpr size_t ERX_MAX = 37;              ///< maximum number of eRx channels

    const unsigned int idle_pattern_;
    const unsigned int header_marker_;

    struct ECOND {
      uint64_t header;
      uint32_t body[PAYLOAD_LENGTH_MAX];
      uint16_t payload_length;
    };
    struct ERX {
      uint64_t header;
      uint8_t channels[ERX_MAX];
      uint8_t type[ERX_MAX];
      uint32_t body[ERX_MAX];
      uint8_t channel_number;
    };
  };
}  // namespace hgcal

#endif
