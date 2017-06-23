#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSTimingCalibration.h"

void
CTPPSTimingCalibration::insert( const unsigned int& chan, const double& offset, const double& width )
{
  // store the offset
  auto it_offs = offsets.find( chan );
  if ( it_offs != offsets.end() ) {
    edm::LogError("CTPPSTimingCalibration") << "Warning: overwriting entry at " << chan << ".\n"
       << " previous: " << it_offs->second << "\n"
       << " new:      " << offset << ". ";
  }
  offsets[chan] = offset;

  // store the width
  auto it_chan = widths.find( chan );
  if ( it_chan != widths.end() ) {
    edm::LogError("CTPPSTimingCalibration") << "Warning: overwriting entry at " << chan << ".\n"
       << " previous: " << it_chan->second << "\n"
       << " new:      " << width << ". ";
  }
  widths[chan] = width;
}

std::set<unsigned int>
CTPPSTimingCalibration::channels() const
{
  std::set<unsigned int> out;
  for ( const auto& it : offsets ) {
    out.insert( it.first );
  }
  return out;
}
