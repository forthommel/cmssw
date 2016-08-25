#ifndef RecoCTPPS_ProtonProducer_ProtonKinematicsUtils_h
#define RecoCTPPS_ProtonProducer_ProtonKinematicsUtils_h

#include "DataFormats/CTPPSReco/interface/ProtonTrack.h"
#include "DataFormats/Provenance/interface/RunID.h"

namespace ProtonUtils
{
    float tracksDistance( const reco::ProtonTrack& near_p, const reco::ProtonTrack& far_p )
    {
        const float z_far = far_p.getZ0();
        const TVector2 far_xy_ext = near_p.getTrackPoint( z_far ), far_xy_obs = TVector2( far_p.getX0(), far_p.getY0() );
        return (far_xy_ext-far_xy_obs).Mod()/1.e3; // mm -> m
    }
}

namespace CTPPSAlCa
{

    struct RPAlignmentConstants {
        RPAlignmentConstants() :
            x_shift_l_n( 0. ), x_shift_l_f( 0. ),
            x_shift_r_n( 0. ), x_shift_r_f( 0. ) {;}
        RPAlignmentConstants( float l_n, float l_f, float r_n, float r_f ) :
            x_shift_l_n( l_n ), x_shift_l_f( l_f ),
            x_shift_r_n( r_n ), x_shift_r_f( r_f ) {;}
        bool valid() const { return ( ( x_shift_l_n!=0. ) && ( x_shift_l_f!=0. ) && ( x_shift_r_n!=0. ) && ( x_shift_r_f!=0. ) ); }

        float x_shift_l_n, x_shift_l_f; // in mm
        float x_shift_r_n, x_shift_r_f; // in mm
    };

    struct RPCalibrationConstants {
        RPCalibrationConstants() :
            x_disp_l_n( 0. ), x_disp_l_f( 0. ),
            x_disp_r_n( 0. ), x_disp_r_f( 0. ) {;}
        RPCalibrationConstants( float l_n, float l_f, float r_n, float r_f ) :
            x_disp_l_n( l_n ), x_disp_l_f( l_f ),
            x_disp_r_n( r_n ), x_disp_r_f( r_f ) {;}

        // dispersions (x-axis)
        float x_disp_l_n, x_disp_l_f;
        float x_disp_r_n, x_disp_r_f;
    };

    RPCalibrationConstants getCalibrationConstants( const edm::RunNumber_t& )
    {
        RPCalibrationConstants cc;
        cc.x_disp_l_f = 9.22e-2;
        cc.x_disp_l_n = 9.26e-2;
        cc.x_disp_r_f = 5.16e-2;
        cc.x_disp_r_n = 5.81e-2;
        return cc;
    }

}

#endif
