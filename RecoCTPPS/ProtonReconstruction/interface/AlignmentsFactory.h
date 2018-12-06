/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef RecoCTPPSAnalysis_ProtonReconstruction_AlignmentsFactory_h
#define RecoCTPPSAnalysis_ProtonReconstruction_AlignmentsFactory_h

#include "DataFormats/Provenance/interface/EventRange.h"

#include <vector>
#include <map>
#include <regex>

namespace pps
{
  struct alignment_t
  {
    double x_align, x_align_err;
    double y_align, y_align_err;
  };
  typedef std::map<unsigned short,alignment_t> alignments_t;
  std::ostream& operator<<( std::ostream& os, const alignment_t al );
  std::ostream& operator<<( std::ostream& os, const alignments_t als );

  class AlignmentsFactory
  {
    public:
      AlignmentsFactory( const std::string& filename );
      ~AlignmentsFactory() = default;

      const alignments_t& get( unsigned short fill_num ) const;

    private:
      struct runbased_alignments_t
      {
        edm::EventRange fill_range;
        alignments_t alignments;
      };
      static const std::regex RGX_HEADER, RGX_ALIGN;

      std::vector<runbased_alignments_t> alignments_;
  };
}

#endif

