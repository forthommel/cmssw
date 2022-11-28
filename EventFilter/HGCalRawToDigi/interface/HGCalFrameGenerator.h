#ifndef EventFilter_HGCalRawToDigi_HGCalFrameGenerator_h
#define EventFilter_HGCalRawToDigi_HGCalFrameGenerator_h

#include "EventFilter/HGCalRawToDigi/interface/SlinkTypes.h"

#include <cstdint>
#include <vector>

namespace edm {
  class ParameterSet;
  class ParameterSetDescription;
}  // namespace edm
namespace CLHEP {
  class HepRandomEngine;
}

namespace hgcal {
  class HGCalFrameGenerator {
  public:
    explicit HGCalFrameGenerator(const edm::ParameterSet&);

    static edm::ParameterSetDescription description();

    void setRandomEngine(CLHEP::HepRandomEngine& rng);

    std::vector<uint32_t> produceECONEvent(const econd::ECONDEvent&) const;

  private:
    std::vector<bool> generateEnabledChannels(uint64_t&) const;
    std::vector<uint32_t> generateERxData(const econd::ERxEvent&, std::vector<uint64_t>&) const;
    struct HeaderBits {
      bool bitO, bitB, bitE, bitT, bitH, bitS;
    };
    HeaderBits generateStatusBits() const;
    /// 8bit CRC for event header
    uint8_t computeCRC(const std::vector<uint32_t>&) const;

    double chan_surv_prob_;
    const std::vector<unsigned int> enabled_channels_;
    const unsigned int header_marker_;
    const unsigned int num_channels_;
    double bitO_error_prob_, bitB_error_prob_, bitE_error_prob_, bitT_error_prob_, bitH_error_prob_, bitS_error_prob_;
    CLHEP::HepRandomEngine* rng_{nullptr};
  };
}  // namespace hgcal

#endif
