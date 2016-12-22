#include "DataFormats/CTPPSReco/interface/ProtonTrack.h"

using namespace reco;

ProtonTrack::ProtonTrack() :
    det_id_( CTPPSDetId( CTPPSDetId::sdTrackingStrip, 0, 0, 0 ) )
{}

ProtonTrack::ProtonTrack( const TotemRPDetId& det_id, const TotemRPLocalTrack& arplt ) :
    TotemRPLocalTrack( arplt ), det_id_( det_id )
{}

ProtonTrack::~ProtonTrack()
{}
