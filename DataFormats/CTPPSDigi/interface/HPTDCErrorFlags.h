/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Seyed Mohsen Etesami
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef DataFormats_CTPPSDigi_HPTDCErrorFlags_h
#define DataFormats_CTPPSDigi_HPTDCErrorFlags_h

#include <string>
#include <vector>

/// HPTDC error flags container
class HPTDCErrorFlags {
public:
  HPTDCErrorFlags(unsigned short flags = 0) : error_flags(flags) {}

  bool errorId(unsigned short pos) const { return (error_flags >> pos) & 0x1; }

  bool internalFatalChipError() const { return errorId(0); }
  void setInternalFatalChipError(bool bit) { setBit(0, bit); }
  bool eventLost() const { return errorId(1); }
  void setEventLost(bool bit) { setBit(1, bit); }
  bool hitRejectedByEventSizeLimit() const { return errorId(2); }
  void setHitRejectedByEventSizeLimit(bool bit) { setBit(2, bit); }

  bool hitErrorGroup3() const { return errorId(3); }
  void setHitErrorGroup3(bool bit) { setBit(3, bit); }
  bool hitLostL1OverflowGroup3() const { return errorId(4); }
  void setHitLostL1OverflowGroup3(bool bit) { setBit(4, bit); }
  bool hitLostROFifoOverflowGroup3() const { return errorId(5); }
  void setHitLostROFifoOverflowGroup3(bool bit) { setBit(5, bit); }

  bool hitErrorGroup2() const { return errorId(6); }
  void setHitErrorGroup2(bool bit) { setBit(6, bit); }
  bool hitLostL1OverflowGroup2() const { return errorId(7); }
  void setHitLostL1OverflowGroup2(bool bit) { setBit(7, bit); }
  bool hitLostROFifoOverflowGroup2() const { return errorId(8); }
  void setHitLostROFifoOverflowGroup2(bool bit) { setBit(8, bit); }

  bool hitErrorGroup1() const { return errorId(9); }
  void setHitErrorGroup1(bool bit) { setBit(9, bit); }
  bool hitLostL1OverflowGroup1() const { return errorId(10); }
  void setHitLostL1OverflowGroup1(bool bit) { setBit(10, bit); }
  bool hitLostROFifoOverflowGroup1() const { return errorId(11); }
  void setHitLostROFifoOverflowGroup1(bool bit) { setBit(11, bit); }

  bool hitErrorGroup0() const { return errorId(12); }
  void setHitErrorGroup0(bool bit) { setBit(12, bit); }
  bool hitLostL1OverflowGroup0() const { return errorId(13); }
  void setHitLostL1OverflowGroup0(bool bit) { setBit(13, bit); }
  bool hitLostROFifoOverflowGroup0() const { return errorId(14); }
  void setHitLostROFifoOverflowGroup0(bool bit) { setBit(14, bit); }

  inline unsigned short errorFlag() const { return error_flags; }

  static std::string hptdcErrorName(const unsigned short id) {
    static const std::vector<std::string> kErrorNames = {"InternalFatalChipError",
                                                         "EventLost",
                                                         "HitRejectedByEventSizeLimit",
                                                         "HitErrorGroup3",
                                                         "HitLostL1OverflowGroup3",
                                                         "HitLostROFifoOverflowGroup3",
                                                         "HitErrorGroup2",
                                                         "HitLostL1OverflowGroup2",
                                                         "HitLostROFifoOverflowGroup2",
                                                         "HitErrorGroup1",
                                                         "HitLostL1OverflowGroup1",
                                                         "HitLostROFifoOverflowGroup1",
                                                         "HitErrorGroup0",
                                                         "HitLostL1OverflowGroup0",
                                                         "HitLostROFifoOverflowGroup0"};

    if (id >= kErrorNames.size())
      return "NONE";
    return kErrorNames.at(id);
  }

private:
  void setBit(unsigned short pos, bool value) { error_flags ^= (-value ^ error_flags) & (1UL << pos); }
  unsigned short error_flags;
};

#endif
