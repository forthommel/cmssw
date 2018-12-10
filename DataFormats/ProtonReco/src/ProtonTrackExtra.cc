/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DataFormats/ProtonReco/interface/ProtonTrackExtra.h"

using namespace reco;

ProtonTrackExtra::ProtonTrackExtra() :
  valid_fit_( false ), method_( ReconstructionMethod::invalid ), sector_( LHCSector::invalid )
{}

ProtonTrackExtra::ProtonTrackExtra( bool valid, const ReconstructionMethod& method, const LHCSector& sector, const LocalTracksList& tracks_list ) :
  valid_fit_( valid ), method_( method ), sector_( sector ), contributing_tracks_( tracks_list )
{}

