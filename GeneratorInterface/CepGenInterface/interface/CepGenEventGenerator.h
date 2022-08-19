// CepGen-CMSSW interfacing module
//   2022, Laurent Forthomme

#include <memory>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "GeneratorInterface/Core/interface/BaseHadronizer.h"

#include "CepGen/Generator.h"

class CepGenEventGenerator : public gen::BaseHadronizer {
public:
  explicit CepGenEventGenerator(const edm::ParameterSet&);

  bool readSettings(int) { return true; }
  bool declareStableParticles(const std::vector<int>&) { return true; }
  bool declareSpecialSettings(const std::vector<std::string>&) { return true; }

  bool initializeForInternalPartons() { return true; }
  bool generatePartonsAndHadronize();
  bool decay() { return true; }  // NOT used - let's call it "design imperfection"
  bool residualDecay() { return true; }

  void finalizeEvent() {}
  void statistics() {}

  const char* classname() const { return "CepGenEventGenerator"; }
  std::vector<std::string> sharedResources() const { return {}; }

private:
  std::unique_ptr<cepgen::Generator> gen_;
  cepgen::ParametersList proc_params_;
};
