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
#include "DataFormats/ProtonReco/interface/ProtonTrackExtraFwd.h"
#include <set>

/**
 * FIXME make use of the general reco::Candidate object, with appropriate set'ters and get'ters
 */

namespace reco
{
  class ProtonTrack : public Track
  {
    public:
      //typedef ProtonTrackExtra::ReconstructionMethod ReconstructionMethod;
      //typedef ProtonTrackExtra::LHCSector LHCSector;
      /// Default constructor
      ProtonTrack();
      /// Constructor from refit parameters, fitted vertex and direction, and longitudinal fractional momentum loss
      ProtonTrack( double chi2, double ndof, const Point& vtx, const Vector& dir, float xi, float xi_unc = 0. );
      /// Longitudinal fractional momentum loss
      float xi() const { return xi_; }
      /// Absolute uncertainty on longitudinal fractional momentum loss
      float xiError() const { return xi_unc_; }

      void setProtonTrackExtra( const ProtonTrackExtraRef& ref ) { pt_extra_ = ref; }
      const ProtonTrackExtraRef& protonTrackExtra() const { return pt_extra_; }

    private:
      float xi_; ///< Longitudinal fractional momentum loss
      float xi_unc_; ///< Absolute uncertainty on longitudinal fractional momentum loss
      ProtonTrackExtraRef pt_extra_; ///< Additional information on proton track
  };
}

#endif
