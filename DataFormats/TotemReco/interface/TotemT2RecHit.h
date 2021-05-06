/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef DataFormats_TotemReco_TotemT2RecHit_h
#define DataFormats_TotemReco_TotemT2RecHit_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

class TotemT2RecHit {
public:
  TotemT2RecHit() = default;
  explicit TotemT2RecHit(const GlobalPoint&, float, float);

  const GlobalPoint centre() const { return centre_; }
  void setTime(float time) { time_ = time; }
  float time() const { return time_; }
  void setTimeUnc(float time_unc) { time_unc_ = time_unc; }
  float timeUnc() const { return time_unc_; }

private:
  /// Tile centre position
  GlobalPoint centre_;
  /// Leading edge time
  float time_;
  /// Time over threshold/pulse width
  float time_unc_;
};

bool operator<(const TotemT2RecHit&, const TotemT2RecHit&);

#endif
