#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSDiamondTimingCalibration.h"

void
CTPPSDiamondTimingCalibration::insert( const unsigned int& chan, const double& offs )
{
  auto it = offsets.find( chan );
  if ( it != offsets.end() ) {
    edm::LogError("CTPPSDiamondTimingCalibration") << "Warning: overwriting entry at " << chan << ".\n"
       << " previous: " << it->second << "\n"
       << " new:      " << offs << ". ";
  }
  offsets[chan] = offs;
}

std::set<unsigned int>
CTPPSDiamondTimingCalibration::channels() const
{
  std::set<unsigned int> out;
  for ( const auto& it : offsets ) {
    out.insert( it.first );
  }
  return out;
}
