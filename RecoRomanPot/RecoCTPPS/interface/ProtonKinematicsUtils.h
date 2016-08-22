#ifndef RecoRomanPot_RecoCTPPS_ProtonKinematicsUtils_h
#define RecoRomanPot_RecoCTPPS_ProtonKinematicsUtils_h

#include "DataFormats/CTPPSReco/interface/Proton.h"
#include "DataFormats/Provenance/interface/RunID.h"

#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"

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

    class XiInterpolator
    {
    public:
        XiInterpolator() :
            igLF_(0), igLN_(0), igRF_(0), igRN_(0),
            isLF_(0), isLN_(0), isRF_(0), isRN_(0)
        {;}
        XiInterpolator( const char* filename ) :
            igLF_(0), igLN_(0), igRF_(0), igRN_(0),
            isLF_(0), isLN_(0), isRF_(0), isRN_(0)
        {
            loadInterpolationGraphs( filename );
        }

        ~XiInterpolator() {
            if ( isLF_ ) delete isLF_;
            if ( isLN_ ) delete isLN_;
            if ( isRF_ ) delete isRF_;
            if ( isRN_ ) delete isRN_;
            if ( igLF_ ) delete igLF_;
            if ( igLN_ ) delete igLN_;
            if ( igRF_ ) delete igRF_;
            if ( igRN_ ) delete igRN_;
        }

        void loadInterpolationGraphs( const char* filename, bool conversion=false ) {
            TFile f( filename );
            if ( !f.IsOpen() ) {
                edm::LogError("ProtonReco") << "Failed to load the interpolation graphs file";
                return;
            }

            if ( conversion ) {
                // in case one works with Frici's input
                extractSpline( (TGraph*)f.Get("XRPH_C6R5_B1"), "x_to_xi_R_1_N", igRN_, isRN_ );
                extractSpline( (TGraph*)f.Get("XRPH_D6R5_B1"), "x_to_xi_R_1_F", igRF_, isRF_ );
                extractSpline( (TGraph*)f.Get("XRPH_C6L5_B2"), "x_to_xi_L_1_N", igLN_, isLN_ );
                extractSpline( (TGraph*)f.Get("XRPH_D6L5_B2"), "x_to_xi_L_1_F", igLF_, isLF_ );
                return;
            }
            // already "processed" curves
            igRN_ = (TGraph*)f.Get("g_x_to_xi_R_1_N");
            igRF_ = (TGraph*)f.Get("g_x_to_xi_R_1_F");
            igLN_ = (TGraph*)f.Get("g_x_to_xi_L_1_N");
            igLF_ = (TGraph*)f.Get("g_x_to_xi_L_1_F");
            isRN_ = (TSpline3*)f.Get("s_x_to_xi_R_1_N");
            isRF_ = (TSpline3*)f.Get("s_x_to_xi_R_1_F");
            isLN_ = (TSpline3*)f.Get("s_x_to_xi_L_1_N");
            isLF_ = (TSpline3*)f.Get("s_x_to_xi_L_1_F");
        }

        void setAlignmentConstants( const edm::RunNumber_t& run_id ) {
            ac_ = getAlignmentConstants( run_id );
            edm::LogInfo("ProtonReco")
                << "Alignment constants loaded: " << ac_.x_shift_l_f << ":" << ac_.x_shift_l_n << " / "
                                                  << ac_.x_shift_r_f << ":" << ac_.x_shift_r_n;
        }
        void setAlignmentConstants( const RPAlignmentConstants& ac ) { ac_ = ac; }

        void computeXiLinear( const reco::Proton& prot, float* xi_, float* err_xi_ ) {
            *xi_ = *err_xi_ = 0.;

            if ( !prot.isValid() ) return;

            float dx_n, dx_f;
            float x_n_shift = 0., x_f_shift = 0.;
            switch ( prot.side() ) {
                case reco::ProtonTrack::LeftSide: {
                    dx_n = 9.26e-2;
                    dx_f = 9.22e-2;
                    x_n_shift = ac_.x_shift_l_n;
                    x_f_shift = ac_.x_shift_l_f;
                } break;
                case reco::ProtonTrack::RightSide: {
                    dx_n = 5.81e-2;
                    dx_f = 5.16e-2;
                    x_n_shift = ac_.x_shift_r_n;
                    x_f_shift = ac_.x_shift_r_f;
                } break;
                default: return;
            }

            const float de_x = 0.2e-3/*m*/, de_rel_dx = 0.1;

            float x_corr = 0.;
            if ( prot.farTrack() && prot.farTrack()->isValid() ) {
                x_corr = ( prot.farTrack()->getX0() + x_f_shift ) * 1.e-3;
                *xi_ = x_corr / dx_f;
                *err_xi_ = std::sqrt( std::pow( de_x/dx_f, 2 )
                                    + std::pow( de_rel_dx * (*xi_), 2 ) );
                return;
            }
            if ( prot.nearTrack() && prot.nearTrack()->isValid() ) {
                x_corr = ( prot.nearTrack()->getX0() + x_n_shift ) * 1.e-3;
                *xi_ = x_corr / dx_n;
                *err_xi_ = std::sqrt( std::pow( de_x/dx_n, 2 )
                                    + std::pow( de_rel_dx * (*xi_), 2 ) );
                return;
            }
        }

        void computeXiSpline( const reco::Proton& prot, float* xi, float* err_xi ) {
            *xi = *err_xi = 0.;

            if ( !prot.isValid() ) return;

            TSpline3 *interp_near = 0, *interp_far = 0;
            float x_n_shift = 0., x_f_shift = 0.;
            switch ( prot.side() ) {
                case reco::ProtonTrack::LeftSide: {
                    interp_far = isLF_;
                    interp_near = isLN_;
                    x_n_shift = ac_.x_shift_l_n;
                    x_f_shift = ac_.x_shift_l_f;
                    break;
                }
                case reco::ProtonTrack::RightSide: {
                    interp_far = isRF_;
                    interp_near = isRN_;
                    x_n_shift = ac_.x_shift_r_n;
                    x_f_shift = ac_.x_shift_r_f;
                    break;
                }
                default: return;
            }
            if ( ( !interp_far ) or ( !interp_near ) ) return;

            const float de_x = 0.4e-3/*m*/, de_rel_dx = 0.1;

            float x_corr = 0.;
            if ( prot.farTrack() && prot.farTrack()->isValid() ) {
                x_corr = ( prot.farTrack()->getX0() + x_f_shift ) * 1.e-3;
                *xi = interp_far->Eval( x_corr );
                const float de_xi = interp_far->Eval( x_corr + de_x );
                *err_xi = std::sqrt( std::pow( de_xi, 2 )
                                   + std::pow( de_rel_dx * (*xi), 2 ) );
                return;
            }
            if ( prot.nearTrack() && prot.nearTrack()->isValid() ) {
                x_corr = ( prot.nearTrack()->getX0() + x_n_shift ) * 1.e-3;
                *xi = interp_near->Eval( x_corr );
                const float de_xi = interp_near->Eval( x_corr + de_x );
                *err_xi = std::sqrt( std::pow( de_xi, 2 )
                                   + std::pow( de_rel_dx * (*xi), 2 ) );
                return;
            }
        }

    private:
        void extractSpline( const TGraph* gr_in, const char* name_out, TGraph* gr_out, TSpline3* sp_out ) {
            if ( !gr_in ) return;

            const double offset = gr_in->GetX()[0];
            if ( gr_out ) delete gr_out;
            gr_out = new TGraph( Form( "g_%s", name_out ), gr_in->GetTitle() );

            double x, y;
            for ( int i=0; i<gr_in->GetN(); i++ ) {
                gr_in->GetPoint( i, x, y );
                gr_out->SetPoint( i, x-offset, -y );
            }
            if ( sp_out ) delete sp_out;
            sp_out = new TSpline3( "", gr_out->GetX(), gr_out->GetY(), gr_out->GetN() );
            sp_out->SetName( Form( "s_%s", name_out ) );
        }

        TGraph *igLF_, *igLN_, *igRF_, *igRN_;
        TSpline3 *isLF_, *isLN_, *isRF_, *isRN_;
        RPAlignmentConstants ac_;
        
    };
}

#endif
