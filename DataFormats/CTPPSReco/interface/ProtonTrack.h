#ifndef DataFormats_CTPPSReco_ProtonTrack_h
#define DataFormats_CTPPSReco_ProtonTrack_h

#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"

namespace reco {

    class ProtonTrack : public TotemRPLocalTrack
    {

    public:
        enum RomanPot { NearHorizontal = 2, FarHorizontal = 3 };
        enum Side { LeftSide = 0, RightSide = 1 };

        ProtonTrack();
        ProtonTrack( const TotemRPDetId&, const TotemRPLocalTrack& );
        ~ProtonTrack();

        ProtonTrack *clone() const { return ( new ProtonTrack( *this ) ); }

        inline unsigned int decDetId() const { return det_id_.rawId(); }
        inline const TotemRPDetId& detId() const { return det_id_; }

        inline RomanPot rp() const { return static_cast<RomanPot>( det_id_.rp() ); }
        inline Side side() const { return static_cast<Side>( det_id_.arm() );  }

    private:
        TotemRPDetId det_id_;
    };
}

#endif
