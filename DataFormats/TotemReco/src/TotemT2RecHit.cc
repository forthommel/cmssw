/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

TotemT2RecHit::TotemT2RecHit(const GlobalPoint& centre, float time, float time_unc)
    : centre_(centre), time_(time), time_unc_(time_unc) {
}

bool operator<(const TotemT2RecHit& lhs, const TotemT2RecHit& rhs) {
  if (lhs.time() < rhs.time())
    return true;
  if (lhs.time() > rhs.time())
    return false;
  if (lhs.timeUnc() < rhs.timeUnc())
    return true;
  if (lhs.timeUnc() > rhs.timeUnc())
    return false;
  return false;
}

