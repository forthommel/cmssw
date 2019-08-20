/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *
 ****************************************************************************/

#ifndef DataFormats_CTPPSReco_CTPPSTimingLocalTrack
#define DataFormats_CTPPSReco_CTPPSTimingLocalTrack

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/CTPPSReco/interface/CTPPSTimingRecHit.h"

//----------------------------------------------------------------------------------------------------

class CTPPSTimingLocalTrack {
public:
  CTPPSTimingLocalTrack();
  CTPPSTimingLocalTrack(const math::XYZPoint& pos0, const math::XYZPoint& pos0_sigma, float t, float t_sigma);
  virtual ~CTPPSTimingLocalTrack() = default;

  enum class CheckDimension { x, y, all };
  bool containsHit(const CTPPSTimingRecHit& recHit,
                   float tolerance = 0.1f,
                   CheckDimension check = CheckDimension::all) const;

  //--- spatial get'ters

  inline float getX0() const { return pos0_.x(); }
  inline float getX0Sigma() const { return pos0_sigma_.x(); }

  inline float getY0() const { return pos0_.y(); }
  inline float getY0Sigma() const { return pos0_sigma_.y(); }

  inline float getZ0() const { return pos0_.z(); }
  inline float getZ0Sigma() const { return pos0_sigma_.z(); }

  inline int getNumOfHits() const { return num_hits_; }
  inline int getNumOfPlanes() const { return num_planes_; }

  //--- spatial set'ters

  inline void setPosition(const math::XYZPoint& pos0) { pos0_ = pos0; }
  inline void setPositionSigma(const math::XYZPoint& pos0_sigma) { pos0_sigma_ = pos0_sigma; }

  inline void setNumOfHits(int num_hits) { num_hits_ = num_hits; }
  inline void setNumOfPlanes(int num_planes) { num_planes_ = num_planes; }

  //--- temporal and general validity flags

  inline virtual void setTimingValid(bool valid) {}
  inline virtual bool isTimingValid() const { return getT() != T_INVALID; }

  inline void setPositionValid(bool valid) { valid_ = valid; }
  inline bool isPositionValid() const { return valid_; }

  inline virtual void setValid(bool valid) { setPositionValid(valid); }
  inline void setValid(bool pos_valid, bool time_valid) {
    setPositionValid(pos_valid);
    setTimingValid(time_valid);
  }
  inline virtual bool isValid() const { return isPositionValid() && isTimingValid(); }

  //--- temporal get'ters

  inline float getT() const { return t_; }
  inline float getTSigma() const { return t_sigma_; }

  //--- temporal set'ters

  inline void setT(float t) { t_ = t; }
  inline void setTSigma(float t_sigma) { t_sigma_ = t_sigma; }

protected:
  static constexpr float T_INVALID = -999.;

private:
  //--- spatial information

  /// initial track position
  math::XYZPoint pos0_;
  /// error on the initial track position
  math::XYZPoint pos0_sigma_;

  /// number of hits participating in the track
  int num_hits_;

  /// number of planes participating in the track
  int num_planes_;

  /// fit valid?
  bool valid_;

  //--- timing information
  float t_;
  float t_sigma_;
};

/// Comparison operator
bool operator<(const CTPPSTimingLocalTrack& lhs, const CTPPSTimingLocalTrack& rhs);

#endif
