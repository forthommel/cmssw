/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef DataFormats_ProtonReco_ProtonTrackExtra_h
#define DataFormats_ProtonReco_ProtonTrackExtra_h

#include <set>
#include "DataFormats/Common/interface/Ref.h"

namespace reco
{
  class ProtonTrackExtra
  {
    public:
      typedef std::set<unsigned int> RPList;
      enum class ReconstructionMethod { invalid = -1, singleRP, multiRP };
      enum class LHCSector { invalid = -1, sector45, sector56 };

      ProtonTrackExtra();
      ProtonTrackExtra( bool valid, const ReconstructionMethod& method, const LHCSector& sector, const RPList& rp_list );

      void setValid( bool valid = true ) { is_valid_ = valid; }
      bool valid() const { return is_valid_; }

      void setMethod( const ReconstructionMethod& method ) { method_ = method; }
      ReconstructionMethod method() const { return method_; }

      void setSector( const LHCSector& sector ) { sector_ = sector; }
      LHCSector sector() const { return sector_; }

      void setContributingRPs( const RPList& list ) { contributing_rp_ids_ = list; }
      const RPList& contributingRPs() const { return contributing_rp_ids_; }

    private:
      // TODO: rename to fit valid?
      bool is_valid_;
      ReconstructionMethod method_;
      LHCSector sector_;
      RPList contributing_rp_ids_;
  };
}

#endif

