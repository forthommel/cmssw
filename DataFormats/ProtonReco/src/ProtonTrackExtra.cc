/****************************************************************************
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 ****************************************************************************/

#include "DataFormats/ProtonReco/interface/ProtonTrackExtra.h"

using namespace reco;

ProtonTrackExtra::ProtonTrackExtra() :
  valid_fit_( false ), method_( ReconstructionMethod::invalid )
{}

ProtonTrackExtra::ProtonTrackExtra( bool valid, const ReconstructionMethod& method, const CTPPSLocalTrackLiteRefVector& cltv ) :
  valid_fit_( valid ), method_( method ), contributing_local_tracks_( cltv )
{}

