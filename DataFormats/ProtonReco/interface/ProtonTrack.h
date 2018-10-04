/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef DataFormats_ProtonReco_ProtonTrack_h
#define DataFormats_ProtonReco_ProtonTrack_h

#include "DataFormats/TrackReco/interface/Track.h"
#include <set>

/**
 * FIXME make use of the general reco::Candidate object, with appropriate set'ters and get'ters
 */

namespace reco
{
  class ProtonTrack : public Track
  {
    public:
      ProtonTrack();
      ProtonTrack( double chi2, double ndof, const Point& vtx, const Vector& dir, float xi, float xi_unc = 0. );

      enum class ReconstructionMethod { singleRP, multiRP };
      enum class LHCSector { sector45, sector56 };

      float xi() const { return xi_; }
      float xiError() const { return xi_unc_; }

      void setValid( bool valid = true ) { isValid_ = valid; }
      bool valid() const { return isValid_; }

      void setMethod( const ReconstructionMethod& method ) { method_ = method; }
      ReconstructionMethod method() const { return method_; }

      void setSector( const LHCSector& sector ) { sector_ = sector; }
      LHCSector sector() const { return sector_; }

      // TODO: add proper getters, setters
      std::set<unsigned int> contributingRPIds;

    private:
      float xi_; ///< Longitudinal fractional momentum loss
      float xi_unc_; ///< Absolute uncertainty on longitudinal fractional momentum loss

      // TODO: rename to fit valid?
      // FIXME move to a ProtonTrackExtraRef?
      bool isValid_;
      ReconstructionMethod method_;
      LHCSector sector_;
  };
}

#endif
