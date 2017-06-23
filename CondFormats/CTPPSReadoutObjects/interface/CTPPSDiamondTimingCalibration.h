#ifndef CondFormats_CTPPSReadoutObjects_CTPPSDiamondTimingCalibration_h
#define CondFormats_CTPPSReadoutObjects_CTPPSDiamondTimingCalibration_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/Serialization/interface/Serializable.h"

#include <map>
#include <set>

class CTPPSDiamondTimingCalibration
{
  public:
    CTPPSDiamondTimingCalibration() {}
    ~CTPPSDiamondTimingCalibration() {}

    std::map<unsigned int,double> offsets;
    std::map<unsigned int,double> widths;
    void insert( const unsigned int& chan, const double& offset, const double& width );
    std::set<unsigned int> channels() const;

  COND_SERIALIZABLE;
};

#endif
