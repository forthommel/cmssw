#include "CondFormats/CTPPSReadoutObjects/src/headers.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSDiamondTimingCalibration.h"
#include <map>

namespace CondFormats_CTPPSPixelObjects {
   struct dictionary {
   std::map<CTPPSPixelFramePosition, CTPPSPixelROCInfo> ROCMapping; 
   std::map<uint32_t, CTPPSPixelROCAnalysisMask> analysisMask;
 };
}

namespace CondFormats_CTPPSReadoutObjects
{
  struct dictionary
  {
    CTPPSDiamondTimingCalibration dtc;
    std::map<unsigned int,double> dtc_map;
  };
}
