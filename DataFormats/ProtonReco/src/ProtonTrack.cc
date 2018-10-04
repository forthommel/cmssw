/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DataFormats/CTPPSReco/ProtonTrack.h"

using namespace reco;

ProtonTrack::ProtonTrack() :
  xi_( 0. ), xi_unc_( 0. ), isValid_( false )
{}

ProtonTrack::ProtonTrack( const LorentzVector& p4, const Point& vtx, float xi, float xi_unc ) :
  RecoCandidate( (Charge)1, p4, vtx, 2212 ), xi_( xi ), xi_unc_( xi_unc )
{}

ProtonTrack*
ProtonTrack::clone() const
{
  return new ProtonTrack( *this );
}

bool
ProtonTrack::overlap( const Candidate& cand ) const
{
  const RecoCandidate* o = dynamic_cast<const RecoCandidate*>( &cand );
  return ( o != nullptr
    && checkOverlap( track(), o->track() ) );
}

