#include "SimTotem/TotemT2Producer/interface/TotemT2TrivialDigitiserAlgorithm.h"
#include "Geometry/ForwardGeometry/interface/TotemGeometry.h"

TotemT2TrivialDigitiserAlgorithm::TotemT2TrivialDigitiserAlgorithm(const edm::ParameterSet& iConfig)
    : TotemT2DigitiserAlgorithm(iConfig) {}

void TotemT2TrivialDigitiserAlgorithm::run(edmNew::DetSetVector<TotemT2Digi>&) {}
