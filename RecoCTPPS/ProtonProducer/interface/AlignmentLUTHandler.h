#ifndef RecoCTPPS_ProtonProducer_AlignmentLUTHandler_h
#define RecoCTPPS_ProtonProducer_AlignmentLUTHandler_h

#include "DataFormats/Provenance/interface/RunID.h"
#include "RecoCTPPS/ProtonProducer/interface/ProtonKinematicsUtils.h"

#include <regex>
#include <fstream>

namespace CTPPSAlCa {

    class AlignmentLUTHandler
    {
        public:
            inline AlignmentLUTHandler( const char* file ) :
                valid_( false )
            {
                // load the alignment constants from external file
                loadConstants( file );
                valid_ = true;
            }

            inline RPAlignmentConstants getAlignmentConstants( const unsigned short& fill_num ) const {
                if ( !valid_ or !align_map_.count( fill_num ) ) return RPAlignmentConstants();
                return align_map_.at( fill_num );
            }

            inline bool isValid() const { return valid_; }

        private:
            // FIXME to be replaced by a DB handler!
            void loadConstants( const char* file )
            {
                std::ifstream f( file );
                if ( !f.is_open() ) return;
                std::string ss;
                //bool has_margin = false, x_method = false;
                unsigned short fill_num = 0;

                std::regex rgx( ".*id=(\\d+),sh_x=(-?[0-9]\\d*(\\.\\d+)?).*" );
                std::smatch match;

                RPAlignmentConstants ac;

                while ( f >> ss ) {
                    size_t pos1 = ss.find( "[" );
                    if ( pos1!=std::string::npos ) { // new fill info
                        if ( ac.valid() ) {
                            align_map_.insert( std::pair<unsigned int,RPAlignmentConstants>( fill_num, ac ) );
                            ac = RPAlignmentConstants(); // reset the constants list
                        }
                        /*has_margin = ( ss.find( "no_margin" )==std::string::npos ); // .5mm extra margin
                        x_method = ( ss.find( "method x" )!=std::string::npos ); // computation method*/
                        size_t pos2 = ss.find( "fill_" ), pos3 = ss.find( "/method" );
                        fill_num = atoi( ss.substr( pos2+5, pos3 ).c_str() );
                        //std::cout << "-----> fill_num=" << fill_num << ", margin:" << has_margin << ",x_method:" << x_method << std::endl;
                        continue;
                    }
                    const std::string s = ss;
                    if ( std::regex_search( s.begin(), s.end(), match, rgx ) ) {
                        const unsigned int rp_id = atoi( match[1].str().c_str() );
                        const float align_x = atof( match[2].str().c_str() );
                        //std::cout << "----> " << rp_id << ":::" << align_x << std::endl;
                        const unsigned short arm = rp_id/100, pot = rp_id%100;
                        if ( arm==0 ) { // left arm
                            if ( pot==2 ) ac.x_shift_l_n = align_x; // near pot
                            else if ( pot==3 ) ac.x_shift_l_f = align_x; // far pot
                        }
                        else if ( arm==1 ) { //right arm
                            if ( pot==2 ) ac.x_shift_r_n = align_x; // near pot
                            else if ( pot==3 ) ac.x_shift_r_f = align_x; // far pot
                        }
                    }
                }
            }

            bool valid_;
            std::map<unsigned int,RPAlignmentConstants> align_map_; // fill# -> parameters
    };

}

#endif
