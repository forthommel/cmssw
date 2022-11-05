#include "EventFilter/HGCalRawToDigi/interface/RawDataUnpackingTools.h"

namespace hgcal {
  namespace econd {
    ChannelData channelData(const std::vector<uint32_t>& cd) {
      ChannelData data;
      //uint16_t tctp, adc, tot, adcm, toa;
      //bool passZS, passZSm1, hasToA, charmode;
      return data;
    }

    ERxSubPacketHeader eRxSubPacketHeader(const std::vector<uint32_t>& esph) {
      ERxSubPacketHeader header;
      //uint16_t stat, ham;
      //bool bitE;
      //uint16_t cm0, cm1;
      //std::vector<bool> chmap;
      return header;
    }

    EventPacketHeader eventPacketHeader(const std::vector<uint32_t>& eph) {
      EventPacketHeader header;
      //uint16_t header, payload;
      //bool bitP, bitE;
      //uint8_t ht, ebo;
      //bool bitM, bitT;
      //uint8_t hamming;
      //uint16_t bx, l1a;
      //uint8_t orb;
      //bool bitS;
      //uint8_t RR, ehCRC;
      return header;
    }
  }  // namespace econd

  namespace backend {
    CaptureBlockHeader captureBlockHeader(const std::vector<uint32_t>& cbh) {
      CaptureBlockHeader header;
      //uint32_t bc, ec, oc;
      //std::vector<uint8_t> econdStatus;
      return header;
    }

    SlinkFrameHeader slinkHeader(const std::vector<uint32_t>& sfh) {
      SlinkFrameHeader header;
      //uint8_t boe, v, r8;
      //uint64_t global_event_id;
      //uint8_t r6;
      //uint32_t content_id, fed_id;
      return header;
    }

    SlinkFrameTrailer slinkTrailer(const std::vector<uint32_t>& sft) {
      SlinkFrameTrailer trailer;
      //uint8_t eoe, daqcrc, trailer_r;
      //uint64_t event_length;
      //uint8_t bxid;
      //uint32_t orbit_id, crc, status;
      return trailer;
    }
  }  // namespace backend
}  // namespace hgcal
