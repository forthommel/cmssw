#ifndef DataFormats_CTPPSDigi_HPTDCErrorFlags_h
#define DataFormats_CTPPSDigi_HPTDCErrorFlags_h

#include <string>

/** \class HPTDCErrorFlags
 *
 * HPTDC error flags container
 *
 * \author Seyed Mohsen Etesami
 * \author Laurent Forthomme
 * July 2016
 */

class HPTDCErrorFlags {
public:
  HPTDCErrorFlags(unsigned short flags = 0) : error_flags(flags) {}

  bool errorId(unsigned short id) const {
    switch (id) {
      case 0:
        return internalFatalChipError();
      case 1:
        return eventLost();
      case 2:
        return hitRejectedByEventSizeLimit();
      case 3:
        return hitErrorGroup3();
      case 4:
        return hitLostL1OverflowGroup3();
      case 5:
        return hitLostROFifoOverflowGroup3();
      case 6:
        return hitErrorGroup2();
      case 7:
        return hitLostL1OverflowGroup2();
      case 8:
        return hitLostROFifoOverflowGroup2();
      case 9:
        return hitErrorGroup1();
      case 10:
        return hitLostL1OverflowGroup1();
      case 11:
        return hitLostROFifoOverflowGroup1();
      case 12:
        return hitErrorGroup0();
      case 13:
        return hitLostL1OverflowGroup0();
      case 14:
        return hitLostROFifoOverflowGroup0();
      default:
        return true;
    }
  }

  bool internalFatalChipError() const { return bit(0); }
  void setInternalFatalChipError(bool bit) { setBit(0, bit); }
  bool eventLost() const { return bit(1); }
  void setEventLost(bool bit) { setBit(1, bit); }
  bool hitRejectedByEventSizeLimit() const { return bit(2); }
  void setHitRejectedByEventSizeLimit(bool bit) { setBit(2, bit); }

  bool hitErrorGroup3() const { return bit(3); }
  void setHitErrorGroup3(bool bit) { setBit(3, bit); }
  bool hitLostL1OverflowGroup3() const { return bit(4); }
  void setHitLostL1OverflowGroup3(bool bit) { setBit(4, bit); }
  bool hitLostROFifoOverflowGroup3() const { return bit(5); }
  void setHitLostROFifoOverflowGroup3(bool bit) { setBit(5, bit); }

  bool hitErrorGroup2() const { return bit(6); }
  void setHitErrorGroup2(bool bit) { setBit(6, bit); }
  bool hitLostL1OverflowGroup2() const { return bit(7); }
  void setHitLostL1OverflowGroup2(bool bit) { setBit(7, bit); }
  bool hitLostROFifoOverflowGroup2() const { return bit(8); }
  void setHitLostROFifoOverflowGroup2(bool bit) { setBit(8, bit); }

  bool hitErrorGroup1() const { return bit(9); }
  void setHitErrorGroup1(bool bit) { setBit(9, bit); }
  bool hitLostL1OverflowGroup1() const { return bit(10); }
  void setHitLostL1OverflowGroup1(bool bit) { setBit(10, bit); }
  bool hitLostROFifoOverflowGroup1() const { return bit(11); }
  void setHitLostROFifoOverflowGroup1(bool bit) { setBit(11, bit); }

  bool hitErrorGroup0() const { return bit(12); }
  void setHitErrorGroup0(bool bit) { setBit(12, bit); }
  bool hitLostL1OverflowGroup0() const { return bit(13); }
  void setHitLostL1OverflowGroup0(bool bit) { setBit(13, bit); }
  bool hitLostROFifoOverflowGroup0() const { return bit(14); }
  void setHitLostROFifoOverflowGroup0(bool bit) { setBit(14, bit); }

  inline unsigned short errorFlag() const { return error_flags; }

  static std::string hptdcErrorName(const unsigned short id) {
    switch (id) {
      case 0:
        return "InternalFatalChipError";
      case 1:
        return "EventLost";
      case 2:
        return "HitRejectedByEventSizeLimit";
      case 3:
        return "HitErrorGroup3";
      case 4:
        return "HitLostL1OverflowGroup3";
      case 5:
        return "HitLostROFifoOverflowGroup3";
      case 6:
        return "HitErrorGroup2";
      case 7:
        return "HitLostL1OverflowGroup2";
      case 8:
        return "HitLostROFifoOverflowGroup2";
      case 9:
        return "HitErrorGroup1";
      case 10:
        return "HitLostL1OverflowGroup1";
      case 11:
        return "HitLostROFifoOverflowGroup1";
      case 12:
        return "HitErrorGroup0";
      case 13:
        return "HitLostL1OverflowGroup0";
      case 14:
        return "HitLostROFifoOverflowGroup0";
      default:
        return "NONE";
    }
  }

private:
  bool bit(unsigned short pos) const { return (error_flags >> pos) & 0x1; }
  void setBit(unsigned short pos, bool value) { error_flags ^= (-value ^ error_flags) & (1UL << pos); }
  unsigned short error_flags;
};

#endif
