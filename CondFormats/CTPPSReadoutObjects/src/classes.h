#include "CondFormats/CTPPSReadoutObjects/src/headers.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSTimingCalibration.h"
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
    CTPPSTimingCalibration dtc;
    std::map<unsigned int,double> dtc_map;
  };
}
