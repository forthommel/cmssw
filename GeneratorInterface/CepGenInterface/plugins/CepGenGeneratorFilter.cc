// CepGen-CMSSW interfacing module
//   2022, Laurent Forthomme

#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "GeneratorInterface/CepGenInterface/interface/CepGenEventGenerator.h"
#include "GeneratorInterface/ExternalDecays/interface/ExternalDecayDriver.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using CepGenGeneratorFilter = edm::GeneratorFilter<CepGenEventGenerator, gen::ExternalDecayDriver>;

DEFINE_FWK_MODULE(CepGenGeneratorFilter);
