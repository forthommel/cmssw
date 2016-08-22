#include "DataFormats/CTPPSReco/interface/ProtonTrack.h"

using namespace reco;

ProtonTrack::ProtonTrack()
{}

ProtonTrack::ProtonTrack( unsigned int dec_det_id, const TotemRPLocalTrack& arplt ) :
    TotemRPLocalTrack(arplt), det_id_( TotemRPDetId::decToRawId( dec_det_id * 10 ) )
{}

ProtonTrack::~ProtonTrack()
{}
