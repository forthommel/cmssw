// CepGen-CMSSW interfacing module
//   2022, Laurent Forthomme

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "GeneratorInterface/CepGenInterface/interface/CepGenEventGenerator.h"

#include "CepGen/Core/Exception.h"
#include "CepGen/Event/Event.h"
#include "CepGen/Generator.h"
#include "CepGen/Modules/ProcessFactory.h"
#include "CepGen/Parameters.h"
#include "CepGen/Process/Process.h"

#include "CepGenAddOns/HepMC2Wrapper/HepMC2EventInterface.h"

cepgen::ParametersList fromCMSSWParameters(const edm::ParameterSet& iConfig) {
  cepgen::ParametersList params;
  for (const auto& param : iConfig.getParameterNames()) {
    if (iConfig.existsAs<bool>(param))
      params.set(param, iConfig.getUntrackedParameter<bool>(param));
    if (iConfig.existsAs<int>(param))
      params.set(param, iConfig.getUntrackedParameter<int>(param));
    if (iConfig.existsAs<unsigned>(param))
      params.set<unsigned long long>(param, iConfig.getUntrackedParameter<unsigned>(param));
    if (iConfig.existsAs<double>(param))
      params.set(param, iConfig.getUntrackedParameter<double>(param));
    if (iConfig.existsAs<std::string>(param))
      params.set(param, iConfig.getUntrackedParameter<std::string>(param));
    if (iConfig.existsAs<std::vector<double> >(param))
      params.set(param, iConfig.getUntrackedParameter<std::vector<double> >(param));
    if (iConfig.existsAs<edm::ParameterSet>(param))
      params.set(param, fromCMSSWParameters(iConfig.getUntrackedParameter<edm::ParameterSet>(param)));
  }
  return params;
}

CepGenEventGenerator::CepGenEventGenerator(const edm::ParameterSet& iConfig)
    : gen::BaseHadronizer(iConfig),
      gen_(new cepgen::Generator(true /* "safe" mode: start without plugins */)),
      proc_params_(fromCMSSWParameters(iConfig.getUntrackedParameter<edm::ParameterSet>("process"))) {
  //produces<ExampleData2>();
  cepgen::utils::Logger::get().level = (cepgen::utils::Logger::Level)iConfig.getUntrackedParameter<int>("verbosity");
  cepgen::loadLibrary("CepGenHepMC2");
  cepgen::loadLibrary("CepGenProcesses");
  gen_->parametersPtr()->setProcess(cepgen::proc::ProcessFactory::get().build(proc_params_));
  if (!gen_->parameters()->hasProcess())
    throw cms::Exception("CepGenEventGenerator") << "Failed to retrieve a process from the configuration";
}

bool CepGenEventGenerator::generatePartonsAndHadronize() {
  gen_->generate(1);
  const auto& cepgen_evt = gen_->parameters()->process().event();
  auto hepmc_evt = HepMC::CepGenEvent(cepgen_evt);
  event() = std::make_unique<HepMC::GenEvent>(hepmc_evt);
  return true;
}
