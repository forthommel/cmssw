/****************************************************************************
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 ****************************************************************************/

#ifndef DataFormats_ProtonReco_ProtonTrackExtra_h
#define DataFormats_ProtonReco_ProtonTrackExtra_h

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

namespace reco
{
  class ProtonTrackExtra
  {
    public:
      using CTPPSLocalTrackLiteRefVector = std::vector<edm::Ref<std::vector<CTPPSLocalTrackLite>>>;

      /// Type of reconstruction for this track
      enum class ReconstructionMethod { invalid = -1, singleRP, multiRP };

      /// Empty (invalid track extra info) constructor
      ProtonTrackExtra();

      /// Default constructor
      ProtonTrackExtra( bool valid, const ReconstructionMethod& method, const CTPPSLocalTrackLiteRefVector& cltv );

      /// Set the flag for the fit validity
      void setValidFit( bool valid = true ) { valid_fit_ = valid; }
      /// Flag for the fit validity
      bool validFit() const { return valid_fit_; }

      /// Set the reconstruction method for this track
      void setMethod( const ReconstructionMethod& method ) { method_ = method; }
      /// Reconstruction method for this track
      ReconstructionMethod method() const { return method_; }

      /// Store the list of RP tracks that contributed to this global track
      void setContributingLocalTracks( const CTPPSLocalTrackLiteRefVector &v ) { contributing_local_tracks_ = v; }
      /// List of RP tracks that contributed to this global track
      const CTPPSLocalTrackLiteRefVector& contributingLocalTracks() const { return contributing_local_tracks_; }

    private:
      bool valid_fit_;
      ReconstructionMethod method_;
      CTPPSLocalTrackLiteRefVector contributing_local_tracks_;
  };
}

#endif

