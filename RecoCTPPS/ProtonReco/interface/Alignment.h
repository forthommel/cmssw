#ifndef RecoCTPPS_ProtonReco_Alignment_h
#define RecoCTPPS_ProtonReco_Alignment_h

#include <fstream>
#include <regex>
#include <map>

namespace ctpps
{
  class Alignment
  {
    public:
      typedef struct { float x, x_unc, y, y_unc; } align_t;

    public:
      Alignment( const char* file ) :
        rgx_header_( "\\[\\w+\\/fill_(\\d+)\\/(.+)" ),
        rgx_align_( "id=(\\d+),sh_x=([0-9.+-]+),sh_x_unc=([0-9.+-]+),sh_y=([0-9.+-]+),sh_y_unc=([0-9.+-]+)" )
      {
        std::ifstream f( file );
        std::string buf;
        std::smatch match;
        unsigned int fill_num = 0;
        while ( getline( f, buf ) ) {
          if ( std::regex_match( buf, match, rgx_header_ ) ) {
            fill_num = atoi( match[1].str().c_str() );
            alignments_.insert( std::make_pair( fill_num, std::map<unsigned int, align_t>() ) );
            continue;
          }
          if ( std::regex_match( buf, match, rgx_align_ ) && fill_num>0 ) {
            auto& last_align = alignments_[fill_num];
            align_t align;
            align.x = atof( match[2].str().c_str() )*1.e-3;
            align.x_unc = atof( match[3].str().c_str() )*1.e-3;
            align.y = atof( match[4].str().c_str() )*1.e-3;
            align.y_unc = atof( match[5].str().c_str() )*1.e-3;
            last_align.insert( std::make_pair( atoi( match[1].str().c_str() ), align ) );
          }
        }
        f.close();
      }
      std::map<unsigned int, align_t> get( unsigned int fill ) {
        const auto& al = alignments_.find( fill );
        if ( al!=alignments_.end() ) return al->second;
        if ( fill<alignments_.begin()->first ) return alignments_.begin()->second;
        if ( fill>alignments_.rbegin()->first ) return alignments_.rbegin()->second;
        return std::map<unsigned int,align_t>();
      }

    private:
      std::regex rgx_header_;
      std::regex rgx_align_;
      std::map<unsigned int,std::map<unsigned int,align_t> > alignments_;
  };

  /*void print() {
    for ( const auto& fill_info : alignments ) {
      cout << ">>> fill #" << fill_info.first << endl;
      for ( const auto& pot_info : fill_info.second ) {
        cout << "    pot " << pot_info.first << " -> alignment: x=" << pot_info.second.x << " +/- " << pot_info.second.x_unc << ", y=" << pot_info.second.y << " +/- " << pot_info.second.y_unc << endl;
      }
    }
  }*/
}

#endif
