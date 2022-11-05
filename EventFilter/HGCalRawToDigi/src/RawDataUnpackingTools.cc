#include "FWCore/Utilities/interface/Exception.h"

#include "EventFilter/HGCalRawToDigi/interface/RawDataDefinitions.h"
#include "EventFilter/HGCalRawToDigi/interface/RawDataUnpackingTools.h"

namespace hgcal {
  namespace econd {
    ChannelData channelData(const std::vector<uint32_t>& cd, bool charmode) {
      ChannelData data;
      data.tctp = (cd.at(0) >> 30) & 0x3;
      if (charmode) {
        data.adc = (cd.at(0) >> 20) & 0x3ff;
        data.tot = (cd.at(0) >> 10) & 0x3ff;
        data.adcm = 0;  //FIXME
        data.toa = (cd.at(0) >> 0) & 0x3ff;
        return data;
      }
      //FIXME normal mode
      switch (data.tctp) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
          break;
      }
      //uint16_t tctp, adc, tot, adcm, toa;
      //bool passZS, passZSm1, hasToA;
      return data;
    }

    ERxSubPacketHeader eRxSubPacketHeader(const std::vector<uint32_t>& esph) {
      if (esph.size() > 2)
        throw cms::Exception("ERxSubPacketHeader")
            << "Invalid number of words for eRx subpacket header (" << esph.size() << ")";
      ERxSubPacketHeader header;
      header.stat = (esph.at(0) >> ECOND_FRAME::ERXSTAT_POS) & ECOND_FRAME::ERXSTAT_MASK;
      header.ham = (esph.at(0) >> ECOND_FRAME::ERXHAM_POS) & ECOND_FRAME::ERXHAM_MASK;
      header.format = (esph.at(0) >> ECOND_FRAME::ERXFORMAT_POS) & 0x1;
      header.bitE = (esph.at(0) >> ECOND_FRAME::ERX_E_POS) & 0x1;  //FIXME ensure logic is correct...
      header.cm0 = (esph.at(0) >> ECOND_FRAME::COMMONMODE0_POS) & ECOND_FRAME::COMMONMODE0_MASK;
      header.cm1 = (esph.at(0) >> ECOND_FRAME::COMMONMODE1_POS) & ECOND_FRAME::COMMONMODE1_MASK;
      header.chmap.resize(37);
      for (size_t i = 32; i < 37; ++i)
        header.chmap[i] = (esph.at(0) >> ECOND_FRAME::CHMAP32_POS) & 0x1;
      for (size_t i = 0; i < 32; ++i)
        header.chmap[i] = (esph.at(1) >> ECOND_FRAME::CHMAP0_POS) & 0x1;
      return header;
    }

    EventPacketHeader eventPacketHeader(const std::vector<uint32_t>& eph) {
      if (eph.size() != 2)
        throw cms::Exception("EventPacketHeader")
            << "Invalid number of words for event packet header (" << eph.size() << ")";
      EventPacketHeader header;
      header.header = (eph.at(0) >> ECOND_FRAME::HEADER_POS) & ECOND_FRAME::HEADER_MASK;
      header.payload = (eph.at(0) >> ECOND_FRAME::PAYLOAD_POS) & ECOND_FRAME::PAYLOAD_MASK;
      header.bitP = (eph.at(0) >> ECOND_FRAME::BITP_POS) & 0x1;
      header.bitE = (eph.at(0) >> ECOND_FRAME::BITE_POS) & 0x1;
      header.ht = (eph.at(0) >> ECOND_FRAME::HT_POS) & ECOND_FRAME::HT_MASK;
      header.ebo = (eph.at(0) >> ECOND_FRAME::EBO_POS) & ECOND_FRAME::EBO_MASK;
      header.bitM = (eph.at(0) >> ECOND_FRAME::BITM_POS) & 0x1;
      header.bitT = (eph.at(0) >> ECOND_FRAME::BITT_POS) & 0x1;
      header.hamming = (eph.at(0) >> ECOND_FRAME::EHHAM_POS) & ECOND_FRAME::EHHAM_MASK;
      header.bx = (eph.at(1) >> ECOND_FRAME::BX_POS) & ECOND_FRAME::BX_MASK;
      header.l1a = (eph.at(1) >> ECOND_FRAME::L1A_POS) & ECOND_FRAME::L1A_MASK;
      header.orb = (eph.at(1) >> ECOND_FRAME::ORBIT_POS) & ECOND_FRAME::ORBIT_MASK;
      header.bitS = (eph.at(1) >> ECOND_FRAME::BITS_POS) & 0x1;
      header.RR = (eph.at(1) >> ECOND_FRAME::RR_POS) & ECOND_FRAME::RR_MASK;
      header.ehCRC = (eph.at(1) >> ECOND_FRAME::EHCRC_POS) & ECOND_FRAME::EHCRC_MASK;
      return header;
    }
  }  // namespace econd

  namespace backend {
    CaptureBlockHeader captureBlockHeader(const std::vector<uint32_t>& cbh) {
      if (cbh.size() != 2)
        throw cms::Exception("CaptureBlockHeader")
            << "Invalid number of words for capture block header (" << cbh.size() << ")";
      CaptureBlockHeader header;
      header.bc = (cbh.at(0) >> BACKEND_FRAME::CAPTUREBLOCK_BC_POS) & BACKEND_FRAME::CAPTUREBLOCK_BC_MASK;
      header.ec = (cbh.at(0) >> BACKEND_FRAME::CAPTUREBLOCK_EC_POS) & BACKEND_FRAME::CAPTUREBLOCK_EC_MASK;
      header.oc = (cbh.at(0) >> BACKEND_FRAME::CAPTUREBLOCK_OC_POS) & BACKEND_FRAME::CAPTUREBLOCK_OC_MASK;
      header.econdStatus.resize(12);  // a bit of bit-gymnastics for ECON-D status
      header.econdStatus[11] = (cbh.at(0) >> 1) & 0x7;
      header.econdStatus[10] = (cbh.at(0) & 0x1) | ((cbh.at(1) >> 30) & 0x3);  // LSB of word-0 with 2 MSBs of word-1
      for (size_t i = 0; i < 10; ++i)
        header.econdStatus[i] = (cbh.at(1) >> (3 * i)) & 0x7;
      return header;
    }

    SlinkFrameHeader slinkHeader(const std::vector<uint32_t>& sfh) {
      if (sfh.size() != 4)
        throw cms::Exception("SlinkFrameHeader")
            << "Invalid number of words for S-link frame header (" << sfh.size() << ")";
      SlinkFrameHeader header;
      header.boe = (sfh.at(0) >> BACKEND_FRAME::SLINK_BOE_POS) & BACKEND_FRAME::SLINK_BOE_MASK;
      header.v = (sfh.at(0) >> BACKEND_FRAME::SLINK_V_POS) & BACKEND_FRAME::SLINK_V_MASK;
      header.r8 = (sfh.at(0) >> BACKEND_FRAME::SLINK_R8_POS) & BACKEND_FRAME::SLINK_R8_MASK;
      header.global_event_id =
          ((sfh.at(0) >> BACKEND_FRAME::SLINK_GLOBAL_EVENTID_MSB_POS) & BACKEND_FRAME::SLINK_GLOBAL_EVENTID_MSB_MASK) +
          ((sfh.at(1) >> 0) & BACKEND_FRAME::SLINK_GLOBAL_EVENTID_LSB_MASK);
      header.r6 = (sfh.at(2) >> BACKEND_FRAME::SLINK_R6_POS) & BACKEND_FRAME::SLINK_R6_MASK;
      header.content_id = (sfh.at(2) >> BACKEND_FRAME::SLINK_CONTENTID_POS) & BACKEND_FRAME::SLINK_CONTENTID_MASK;
      header.fed_id = (sfh.at(3) >> BACKEND_FRAME::SLINK_SOURCEID_POS) & BACKEND_FRAME::SLINK_SOURCEID_MASK;
      return header;
    }

    SlinkFrameTrailer slinkTrailer(const std::vector<uint32_t>& sft) {
      if (sft.size() != 4)
        throw cms::Exception("SlinkFrameTrailer")
            << "Invalid number of words for S-link frame trailer (" << sft.size() << ")";
      SlinkFrameTrailer trailer;
      trailer.eoe = (sft.at(0) >> BACKEND_FRAME::SLINK_EOE_POS) & BACKEND_FRAME::SLINK_EOE_MASK;
      trailer.daqcrc = (sft.at(0) >> BACKEND_FRAME::SLINK_DAQCRC_POS) & BACKEND_FRAME::SLINK_DAQCRC_MASK;
      trailer.trailer_r = (sft.at(0) >> BACKEND_FRAME::SLINK_TRAILERR_POS) & BACKEND_FRAME::SLINK_TRAILERR_MASK;
      trailer.event_length = (sft.at(1) >> BACKEND_FRAME::SLINK_EVLENGTH_POS) & BACKEND_FRAME::SLINK_EVLENGTH_MASK;
      trailer.bxid = (sft.at(1) >> BACKEND_FRAME::SLINK_BXID_POS) & BACKEND_FRAME::SLINK_BXID_MASK;
      trailer.orbit_id = (sft.at(2) >> BACKEND_FRAME::SLINK_ORBID_POS) & BACKEND_FRAME::SLINK_ORBID_MASK;
      trailer.crc = (sft.at(3) >> BACKEND_FRAME::SLINK_CRC_POS) & BACKEND_FRAME::SLINK_CRC_MASK;
      trailer.status = (sft.at(3) >> BACKEND_FRAME::SLINK_STATUS_POS) & BACKEND_FRAME::SLINK_STATUS_MASK;
      return trailer;
    }
  }  // namespace backend
}  // namespace hgcal
