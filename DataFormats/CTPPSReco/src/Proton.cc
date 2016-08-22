#include "DataFormats/CTPPSReco/interface/Proton.h"

using namespace reco;

Proton::Proton() :
    valid_(false)
{}

Proton::~Proton()
{}

Proton::Proton( const ProtonTrack& atrack, const ProtonTrack::Side& aside ) :
    valid_(true), side_(aside)
{
    tracks_.push_back( atrack );
}

Proton::Proton( const ProtonTrack& aneartrack, const ProtonTrack& afartrack, const ProtonTrack::Side& aside ) :
    valid_(true), side_(aside)
{
    tracks_.push_back( afartrack );
    tracks_.push_back( aneartrack );
}
