/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
*   Jan Kašpar (jan.kaspar@cern.ch)
*
****************************************************************************/

#include "CondFormats/PPSObjects/interface/TotemSymbId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"

void TotemSymbID::print(std::ostream& os, std::string subSystemName) const {
  const auto ctppsDetId = CTPPSDetId(symbolicID);
  os << "DetId=" << symbolicID << " (";

  if (subSystemName == "TrackingStrip") {
    os << "subDet=" << ctppsDetId.subdetId() << " " << TotemRPDetId(symbolicID);
  } else if (subSystemName == "TimingDiamond") {
    os << "subDet=" << ctppsDetId.subdetId() << " " << CTPPSDiamondDetId(symbolicID);
  } else {
    os << ctppsDetId;
  }

  os << ")";
}

std::ostream& operator<<(std::ostream& s, const TotemSymbID& sid) {
  s << "DetId=" << sid.symbolicID << " (" << CTPPSDetId(sid.symbolicID) << ")";

  return s;
}
