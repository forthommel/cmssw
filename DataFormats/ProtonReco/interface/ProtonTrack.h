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

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <unordered_map>

namespace reco
{
  class ProtonTrack : public RecoCandidate
  {
    public:
      enum class FittingMethod { singleRP, multiRP };
      enum class LHCSector { sector45, sector56 };
      typedef std::unordered_map<uint32_t,TrackRef> DetTrackMap;

    public:
      ProtonTrack();
      ProtonTrack( const LorentzVector& p4, const Point& vtx, float xi, float xi_unc=0. );
      ~ProtonTrack() = default;

      /// Returns a clone of the track candidate
      ProtonTrack* clone() const override;

      void setXi( float xi ) { xi_ = xi; }
      float xi() const { return xi_; }

      int numberOfRPs() const { return contributing_tracks_.size(); }

      void setTrack( const TrackRef& track ) { global_track_ = track; }
      TrackRef track() const override { return global_track_; }

      const DetTrackMap& contributingTracks() const { return contributing_tracks_; }
      DetTrackMap& contributingTracks() { return contributing_tracks_; }

      void setFittingMethod( FittingMethod method ) { method_ = method; }
      FittingMethod fittingMethod() const { return method_; }

      void setLHCSector( LHCSector sector ) { lhc_sector_ = sector; }
      LHCSector lhcSector() const { return lhc_sector_; }

    private:
      bool overlap( const Candidate& ) const override;
      /// Proton fractional longitudinal momentum loss
      float xi_;
      /// Uncertainty on the fractional longitudinal momentum loss
      float xi_unc_;
      /// List of detectors contributing to the fitting of this track
      DetTrackMap contributing_tracks_;
      TrackRef global_track_;
      FittingMethod method_;
      LHCSector lhc_sector_;
  };
}

#endif
