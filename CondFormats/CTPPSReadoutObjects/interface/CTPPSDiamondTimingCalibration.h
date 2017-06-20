#ifndef CondFormats_CTPPSReadoutObjects_CTPPSDiamondTimingCalibration_h
#define CondFormats_CTPPSReadoutObjects_CTPPSDiamondTimingCalibration_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/Serialization/interface/Serializable.h"

class CTPPSDiamondTimingCalibration
{
  public:
    std::map<unsigned int,double> offsets;
    void insert( const unsigned int& chan, const double& offs );
    std::set<unsigned int> channels() const;

  private:
    COND_SERIALIZABLE;
};

#endif
