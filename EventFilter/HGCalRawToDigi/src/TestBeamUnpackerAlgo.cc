#include "EventFilter/HGCalRawToDigi/interface/TestBeamUnpackerAlgo.h"

namespace hgcal {
  TestBeamUnpackerAlgo::TestBeamUnpackerAlgo(const edm::ParameterSet& iConfig) : UnpackerAlgo(iConfig) {}

  void TestBeamUnpackerAlgo::run(const FEDRawData&, HGCalDigiCollection&) {}
}  // namespace hgcal
