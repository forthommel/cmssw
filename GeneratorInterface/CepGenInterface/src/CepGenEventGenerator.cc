// CepGen-CMSSW interfacing module
//   2022-2023, Laurent Forthomme

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "GeneratorInterface/CepGenInterface/interface/CepGenEventGenerator.h"
#include "GeneratorInterface/CepGenInterface/interface/CepGenParametersConverter.h"

#include <CepGen/Core/Exception.h>
#include <CepGen/Event/Event.h>
#include <CepGen/Generator.h>
#include <CepGen/Modules/ProcessFactory.h>
#include <CepGen/Parameters.h>
#include <CepGen/Process/Process.h>
#include <CepGenAddOns/HepMC2Wrapper/HepMC2EventInterface.h>

using namespace gen;

CepGenEventGenerator::CepGenEventGenerator(const edm::ParameterSet& iConfig, edm::ConsumesCollector&& iC)
    : BaseHadronizer(iConfig),
      proc_params_(cepgen::fromParameterSet(iConfig.getParameter<edm::ParameterSet>("process"))) {
  // specify the overall module verbosity
  cepgen::utils::Logger::get().setLevel(
      (cepgen::utils::Logger::Level)iConfig.getUntrackedParameter<int>("verbosity", 0));

  // load all required add-ons
  cepgen::loadLibrary("CepGenHepMC2");
  cepgen::loadLibrary("CepGenProcesses");

  // build the process
  edm::LogInfo("CepGenEventGenerator") << "Process to be generated: " << proc_params_ << ".";

  const auto output_params = cepgen::fromParameterSet(iConfig.getParameter<edm::ParameterSet>("outputModules"));
  edm::LogInfo("CepGenEventGenerator") << "Output modules: " << output_params << ".";

  src_ = iC.consumes<CrossingFrame<edm::HepMCProduct> >(
      iConfig.getUntrackedParameter<edm::InputTag>("backgroundLabel", edm::InputTag("mix", "generatorSmeared")));
}

CepGenEventGenerator::~CepGenEventGenerator() { edm::LogInfo("CepGenEventGenerator") << "Destructor called."; }

bool CepGenEventGenerator::initializeForInternalPartons() {
  gen_ = new cepgen::Generator(true /* "safe" mode: start without plugins */);
  gen_->parametersPtr()->setProcess(cepgen::ProcessFactory::get().build(proc_params_));
  if (!gen_->parameters()->hasProcess())
    throw cms::Exception("CepGenEventGenerator") << "Failed to retrieve a process from the configuration";
  return true;
}

bool CepGenEventGenerator::generatePartonsAndHadronize() {
  gen_->generate(1);
  const auto& cepgen_evt = gen_->parameters()->process().event();
  auto hepmc_evt = HepMC::CepGenEvent(cepgen_evt);
  event() = std::make_unique<HepMC::GenEvent>(hepmc_evt);
  return true;
}
