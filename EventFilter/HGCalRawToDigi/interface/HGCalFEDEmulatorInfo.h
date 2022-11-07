#ifndef EventFilter_HGCalRawToDigi_HGCalFEDEmulatorInfo_h
#define EventFilter_HGCalRawToDigi_HGCalFEDEmulatorInfo_h

#include <bitset>

class HGCalFEDEmulatorInfo {
public:
  HGCalFEDEmulatorInfo() = default;
  explicit HGCalFEDEmulatorInfo(uint64_t poi, bool obit, bool bbit, bool ebit, bool tbit, bool hbit, bool sbit)
      : poi_(poi) {
    header_bits_[StatusBits::O] = obit;
    header_bits_[StatusBits::B] = bbit;
    header_bits_[StatusBits::E] = ebit;
    header_bits_[StatusBits::T] = tbit;
    header_bits_[StatusBits::H] = hbit;
    header_bits_[StatusBits::S] = sbit;
  }

  bool channelEnabled(size_t ch_id) const { return poi_.test(ch_id); }
  bool oBit() const { return header_bits_.test(StatusBits::O); }
  bool bBit() const { return header_bits_.test(StatusBits::B); }
  bool eBit() const { return header_bits_.test(StatusBits::E); }
  bool tBit() const { return header_bits_.test(StatusBits::T); }
  bool hBit() const { return header_bits_.test(StatusBits::H); }
  bool sBit() const { return header_bits_.test(StatusBits::S); }

private:
  enum StatusBits { O = 0, B, E, T, H, S };
  std::bitset<37> poi_;
  std::bitset<6> header_bits_;
};

#endif
