#include "SimPPS/TotemT2DigiProducer/interface/TotemT2DigitiserAlgorithm.h"

TotemT2DigitiserAlgorithm::TotemT2DigitiserAlgorithm(const edm::ParameterSet& iConfig) {}

TotemT2DigitiserAlgorithm::~TotemT2DigitiserAlgorithm() {}

void TotemT2DigitiserAlgorithm::setRunConditions(const CTPPSGeometry& geom) { geom_ = &geom; }

void TotemT2DigitiserAlgorithm::addHits(const std::vector<PCaloHit>& hits, int bx) {
  auto& bx_hits = hits_vs_bx_[bx];
  bx_hits.insert(bx_hits.end(), hits.begin(), hits.end());
}

void TotemT2DigitiserAlgorithm::reset() { hits_vs_bx_.clear(); }
