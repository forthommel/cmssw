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

#include <bitset>
#include <string>
#include <vector>

/// HPTDC error flags container
class HPTDCErrorFlags : public std::bitset<15> {
public:
  explicit HPTDCErrorFlags(unsigned short flags = 0) : std::bitset<15>::bitset(flags) {}

  bool errorId(unsigned short id) const { return test(id); }

  bool internalFatalChipError() const { return test(0); }
  void setInternalFatalChipError(bool bit) { set(0, bit); }
  bool eventLost() const { return test(1); }
  void setEventLost(bool bit) { set(1, bit); }
  bool hitRejectedByEventSizeLimit() const { return test(2); }
  void setHitRejectedByEventSizeLimit(bool bit) { set(2, bit); }

  bool hitErrorGroup3() const { return test(3); }
  void setHitErrorGroup3(bool bit) { set(3, bit); }
  bool hitLostL1OverflowGroup3() const { return test(4); }
  void setHitLostL1OverflowGroup3(bool bit) { set(4, bit); }
  bool hitLostROFifoOverflowGroup3() const { return test(5); }
  void setHitLostROFifoOverflowGroup3(bool bit) { set(5, bit); }

  bool hitErrorGroup2() const { return test(6); }
  void setHitErrorGroup2(bool bit) { set(6, bit); }
  bool hitLostL1OverflowGroup2() const { return test(7); }
  void setHitLostL1OverflowGroup2(bool bit) { set(7, bit); }
  bool hitLostROFifoOverflowGroup2() const { return test(8); }
  void setHitLostROFifoOverflowGroup2(bool bit) { set(8, bit); }

  bool hitErrorGroup1() const { return test(9); }
  void setHitErrorGroup1(bool bit) { set(9, bit); }
  bool hitLostL1OverflowGroup1() const { return test(10); }
  void setHitLostL1OverflowGroup1(bool bit) { set(10, bit); }
  bool hitLostROFifoOverflowGroup1() const { return test(11); }
  void setHitLostROFifoOverflowGroup1(bool bit) { set(11, bit); }

  bool hitErrorGroup0() const { return test(12); }
  void setHitErrorGroup0(bool bit) { set(12, bit); }
  bool hitLostL1OverflowGroup0() const { return test(13); }
  void setHitLostL1OverflowGroup0(bool bit) { set(13, bit); }
  bool hitLostROFifoOverflowGroup0() const { return test(14); }
  void setHitLostROFifoOverflowGroup0(bool bit) { set(14, bit); }

  inline unsigned short errorFlag() const { return to_ulong(); }

  static std::string hptdcErrorName(const unsigned short id) {
    static const std::vector<std::string> kErrorNames = {
        "InternalFatalChipError",
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
        "HitLostROFifoOverflowGroup0",
    };
    if (id >= kErrorNames.size())
      return "NONE";
    return kErrorNames.at(id);
  }
};

#endif
