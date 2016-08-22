#ifndef RecoCTPPS_ProtonProducer_ProtonKinematicsUtils_h
#define RecoCTPPS_ProtonProducer_ProtonKinematicsUtils_h

#include "DataFormats/CTPPSReco/interface/ProtonTrack.h"
#include "DataFormats/Provenance/interface/RunID.h"

#include <iostream>

namespace ProtonUtils {

    float tracksDistance( const reco::ProtonTrack& near_p, const reco::ProtonTrack& far_p )
    {
        const float z_far = far_p.getZ0();
        const TVector2 far_xy_ext = near_p.getTrackPoint( z_far ), far_xy_obs = TVector2( far_p.getX0(), far_p.getY0() );
        return (far_xy_ext-far_xy_obs).Mod()/1.e3; // mm -> m
    }

    struct RPAlignmentConstants {
        RPAlignmentConstants() : x_shift_l_n(0.), x_shift_l_f(0.), x_shift_r_n(0.), x_shift_r_f(0.) {;}
        float x_shift_l_n, x_shift_l_f; // in mm
        float x_shift_r_n, x_shift_r_f; // in mm
    };

    RPAlignmentConstants getAlignmentConstants( const edm::RunNumber_t& run_id )
    {
        // all alignment constants are given in mm
        RPAlignmentConstants ac;
        if ( run_id<274244 ) {
            ac.x_shift_l_f = -3.40;
            ac.x_shift_l_n = -0.90;
            ac.x_shift_r_f = -2.40;
            ac.x_shift_r_n = -2.75;
        }
        else {
            ac.x_shift_l_f = -3.90;
            ac.x_shift_l_n = -1.45;
            ac.x_shift_r_f = -2.85;
            ac.x_shift_r_n = -3.25;
        }
        return ac;
    }

    struct RPCalibrationConstants {
        RPCalibrationConstants() : x_disp_l_n(0.), x_disp_l_f(0.), x_disp_r_n(0.), x_disp_r_f(0.) {;}
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
