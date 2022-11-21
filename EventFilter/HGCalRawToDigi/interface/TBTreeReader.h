#ifndef EventFilter_HGCalRawToDigi_TBTreeReader_h
#define EventFilter_HGCalRawToDigi_TBTreeReader_h

#include "TChain.h"

namespace hgcal::econd {
  class TBTreeReader {
  public:
    /// \param[in] tree_name Name of the TB events tree
    /// \param[in] filenames List of filenames to loop on
    /// \param[in] num_channels Channels multiplicity
    explicit TBTreeReader(const std::string& tree_name,
                          const std::vector<std::string>& filenames,
                          unsigned int num_channels);

    /// Input tree collections
    struct TreeEvent {
      int event, chip, half, channel, adc, adcm, toa, tot, totflag, bxcounter, eventcounter, orbitcounter;
    };
    /// Event index (L1A/BX/orbit)
    typedef std::tuple<uint32_t, uint32_t, uint32_t> EventId;
    typedef std::tuple<uint8_t, uint8_t> ERx_t;
    struct ERxData_t {
      std::vector<uint16_t> adc, adcm, toa, tot;
      std::vector<uint8_t> tctp;
      uint32_t cm0, cm1;
    };
    typedef std::map<EventId, std::map<ERx_t, ERxData_t>> ECONDInputs_t;
    typedef std::pair<EventId, std::map<ERx_t, ERxData_t>> ECONDEvent;
    ECONDEvent next();

  private:
    TChain chain_;
    ECONDInputs_t data_;
    ECONDInputs_t::const_iterator it_data_;
    unsigned int num_channels_;
  };
}  // namespace hgcal::econd

#endif
