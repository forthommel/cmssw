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
  is_valid_( false ), method_( ReconstructionMethod::invalid ), sector_( LHCSector::invalid )
{}

ProtonTrackExtra::ProtonTrackExtra( bool valid, const ReconstructionMethod& method, const LHCSector& sector, const RPList& rp_list ) :
  is_valid_( valid ), method_( method ), sector_( sector ), contributing_rp_ids_( rp_list )
{}

