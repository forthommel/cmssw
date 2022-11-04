#ifndef EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h
#define EventFilter_HGCalRawToDigi_TestBeamUnpackerAlgo_h

#include "EventFilter/HGCalRawToDigi/interface/HGCalUnpackerAlgo.h"

namespace hgcal {
  class TestBeamUnpackerAlgo final : public UnpackerAlgo {
  public:
    explicit TestBeamUnpackerAlgo(const edm::ParameterSet&);

    void run(const FEDRawData&, HGCalDigiCollection&) override;

  private:
  };
}  // namespace hgcal

#endif
