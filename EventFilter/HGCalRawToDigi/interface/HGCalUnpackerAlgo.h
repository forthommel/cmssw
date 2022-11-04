#ifndef EventFilter_HGCalRawToDigi_HGCalUnpackerAlgo_h
#define EventFilter_HGCalRawToDigi_HGCalUnpackerAlgo_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/HGCDigi/interface/HGCDigiCollections.h"

#include "EventFilter/HGCalRawToDigi/interface/RawDataDefinitions.h"

namespace hgcal {
  class UnpackerAlgo {
  public:
    explicit UnpackerAlgo(const edm::ParameterSet&) {}
    virtual ~UnpackerAlgo() = default;

    virtual void run(const FEDRawData&, HGCalDigiCollection&) = 0;
  };
}  // namespace hgcal

#endif
