/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#ifndef DataFormats_CTPPSReco_CTPPSTimingRecHit
#define DataFormats_CTPPSReco_CTPPSTimingRecHit

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

/// Reconstructed hit in timing detectors.
namespace io_v1 {
  class CTPPSTimingRecHit {
  public:
    CTPPSTimingRecHit() {}
    explicit CTPPSTimingRecHit(float x, float xWidth, float y, float yWidth, float z, float zWidth, float t)
        : x_(x), xWidth_(xWidth), y_(y), yWidth_(yWidth), z_(z), zWidth_(zWidth), t_(t) {}

    inline void setDetId(CTPPSDetId detid) { detid_ = detid; }
    inline CTPPSDetId detId() const { return detid_; }

    inline void setX(float x) { x_ = x; }
    inline float x() const { return x_; }

    inline void setY(float y) { y_ = y; }
    inline float y() const { return y_; }

    inline void setZ(float z) { z_ = z; }
    inline float z() const { return z_; }

    inline void setXWidth(float xWidth) { xWidth_ = xWidth; }
    inline float xWidth() const { return xWidth_; }

    inline void setYWidth(float yWidth) { yWidth_ = yWidth; }
    inline float yWidth() const { return yWidth_; }

    inline void setZWidth(float zWidth) { zWidth_ = zWidth; }
    inline float zWidth() const { return zWidth_; }

    inline void setTime(float t) { t_ = t; }
    inline float time() const { return t_; }

  protected:
    CTPPSDetId detid_{CTPPSDetId(CTPPSDetId::sdTimingDiamond, 0, 0)};
    float x_{0.f}, xWidth_{0.f};
    float y_{0.f}, yWidth_{0.f};
    float z_{0.f}, zWidth_{0.f};
    float t_{0.f};
  };

  //----------------------------------------------------------------------------------------------------

  inline bool operator<(const CTPPSTimingRecHit &l, const CTPPSTimingRecHit &r) {
    // only sort by leading edge time
    return (l.time() < r.time());
  }
}  // namespace io_v1
using CTPPSTimingRecHit = io_v1::CTPPSTimingRecHit;
#endif
