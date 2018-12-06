/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "RecoCTPPS/ProtonReconstruction/interface/AlignmentsFactory.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <fstream>

using namespace pps;

namespace pps
{
  std::ostream&
  operator<<( std::ostream& os, const alignment_t al )
  {
    return os
      << "{x=" << al.x_align << "+/-" << al.x_align_err
      << ";y=" << al.y_align << "+/-" << al.y_align_err << "}";
  }
  std::ostream&
  operator<<( std::ostream& os, const alignments_t als )
  {
    os << "alignments[";
    for ( const auto& al : als )
      os << "\npot=" << al.first << ",align=" << al.second;
    return os << "]\n";
  }
}

const std::regex AlignmentsFactory::RGX_HEADER( "\\[\\w+\\/fill_(\\d+)\\/.+\\]" );
const std::regex AlignmentsFactory::RGX_ALIGN( "id=(\\d+),sh_x=([0-9.+-]+)(?:,sh_x_unc=([0-9.+-]+),sh_y=([0-9.+-]+),sh_y_unc=([0-9.+-]+))?" );

AlignmentsFactory::AlignmentsFactory( const std::string& filename )
{
  std::ifstream file( filename );
  if ( !file.is_open() )
    throw cms::Exception( "pps:AlignmentsFactory" )
      << "Failed to open alignment file \"" << filename << "\".";

  unsigned short fill_beg = 0;
  runbased_alignments_t last_alignments;
  std::string line;
  std::smatch match;

  while ( true ) {
    std::getline( file, line );
    if ( ( fill_beg > 0 && std::regex_match( line, match, RGX_HEADER ) && match.size() == 2 ) || file.eof() ) {
      //--- new fill information, or reached the end of file
      const unsigned short fill_end = file.eof()
        ? edm::EventID::maxRunNumber()
        : std::stoi( match[1].str() );
      last_alignments.fill_range = edm::EventRange( fill_beg, 1, 1, fill_end, edm::EventID::maxLuminosityBlockNumber(), edm::EventID::maxEventNumber() );
      alignments_.emplace_back( last_alignments );
      //--- prepare for the next fill
      last_alignments.alignments.clear();
      fill_beg = fill_end;
    }
    else if ( std::regex_match( line, match, RGX_ALIGN ) && match.size() > 0 ) {
      //--- parse new alignment parameters
      const unsigned short pot = std::stoi( match[1].str() );
      const float align_x     = match.size() > 2 ? std::stof( match[2].str() ) : 0.;
      const float err_align_x = match.size() > 3 ? std::stof( match[3].str() ) : 0.;
      const float align_y     = match.size() > 4 ? std::stof( match[4].str() ) : 0.;
      const float err_align_y = match.size() > 5 ? std::stof( match[5].str() ) : 0.;
      last_alignments.alignments[pot] = { align_x, err_align_x, align_y, err_align_y };
    }
    else
      throw cms::Exception( "pps:AlignmentsFactory" )
        << "Failed to parse the following line:\n\t" << line;
  }
}

const alignments_t&
AlignmentsFactory::get( unsigned short fill_num ) const
{
  edm::EventID tmp( fill_num, 1, 1 );
  const auto& it = std::find_if( alignments_.begin(), alignments_.end(), [&tmp]( const runbased_alignments_t& rb_align ) {
    return edm::contains( rb_align.fill_range, tmp );
  } );
  if ( it != alignments_.end() )
    return it->alignments;
  throw cms::Exception( "pps:AlignmentsFactory" )
    << "Failed to retrieve alignment parameters for fill #" << fill_num << ".";
}

