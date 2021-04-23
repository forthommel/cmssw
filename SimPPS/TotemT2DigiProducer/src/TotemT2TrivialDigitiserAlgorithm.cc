#include "SimPPS/TotemT2DigiProducer/interface/TotemT2TrivialDigitiserAlgorithm.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"

TotemT2TrivialDigitiserAlgorithm::TotemT2TrivialDigitiserAlgorithm(const edm::ParameterSet& iConfig)
    : TotemT2DigitiserAlgorithm(iConfig) {}

void TotemT2TrivialDigitiserAlgorithm::run(edm::DetSetVector<TotemT2Digi>&) {}

