#ifndef EventFilter_HGCalRawToDigi_RawDataUnpackingTools_h
#define EventFilter_HGCalRawToDigi_RawDataUnpackingTools_h

#include <cstdint>
#include <vector>

namespace hgcal {
  namespace econd {
    struct ChannelData {
      uint16_t tctp, adc, tot, adcm, toa;
      bool passZS, passZSm1, hasToA, charmode;
    };
    /**
     \short Unpacks the ROC data from the ECON-D format dependending on
       - characterization mode : (TcTp + ADC + TOT + TOA) fixed 32b
       - normal mode : size and fields depend on the TcTb flags
     \note based on Fig. 20 of ECON-D specifications
     \param[in] cd a vector of channel data words (up to 2)
    */
    ChannelData channelData(const std::vector<uint32_t>& cd);

    struct ERxSubPacketHeader {
      uint16_t stat, ham;
      bool bitE;
      uint16_t cm0, cm1;
      std::vector<bool> chmap;
    };
    /**
     \short Unpacks the words for a eRx header
     \note based on Fig. 33 of ECON-D specifications
     \param[in] esph a vector with 1 or 2 32b words
     */
    ERxSubPacketHeader eRxSubPacketHeader(const std::vector<uint32_t>& esph);

    struct EventPacketHeader {
      uint16_t header, payload;
      bool bitP, bitE;
      uint8_t ht, ebo;
      bool bitM, bitT;
      uint8_t hamming;
      uint16_t bx, l1a;
      uint8_t orb;
      bool bitS;
      uint8_t RR, ehCRC;
    };
    /**
     \short Unpacks the two ECON-D header words
     \note based on Fig. 33 of the ECON-D specs
     \param[in] eph a vector of size 2 with the ECON-D header
    */
    EventPacketHeader eventPacketHeader(const std::vector<uint32_t>& eph);
  }  // namespace econd

  namespace backend {
    struct CaptureBlockHeader {
      uint32_t bc, ec, oc;
      std::vector<uint8_t> econdStatus;
    };
    /**
     \short Unpacks the capture block header (see page 16 of "HGCAL BE DAQ firmware description")
     \param[in] cbh a vector of size 2 with the 2 32b words of the capture block header
    */
    CaptureBlockHeader captureBlockHeader(const std::vector<uint32_t>& cbh);

    struct SlinkFrameHeader {
      uint8_t boe, v, r8;
      uint64_t global_event_id;
      uint8_t r6;
      uint32_t content_id, fed_id;
    };
    /**
     \short Unpacks the slink frame header (128 bits header = 4 words)
     \param[in] sfh a vector with 4 32b words
    */
    SlinkFrameHeader slinkHeader(const std::vector<uint32_t>& sfh);

    struct SlinkFrameTrailer {
      uint8_t eoe, daqcrc, trailer_r;
      uint64_t event_length;
      uint8_t bxid;
      uint32_t orbit_id, crc, status;
    };
    /**
     \short Unpacks the slink frame trailer (128 bits trailer = 4 words)
     \param[in] sft a vector with 4 32b words
    */
    SlinkFrameTrailer slinkTrailer(const std::vector<uint32_t>& sft);
  }  // namespace backend
}  // namespace hgcal

#endif
