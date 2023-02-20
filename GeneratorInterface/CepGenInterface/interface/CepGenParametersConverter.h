// CepGen-CMSSW interfacing module
//   2022-2023, Laurent Forthomme

#ifndef GeneratorInterface_CepGenInterface_CepGenParametersConverter_h
#define GeneratorInterface_CepGenInterface_CepGenParametersConverter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace cepgen {
  ParametersList fromParameterSet(const edm::ParameterSet& iConfig) {
    ParametersList params;
    for (const auto& param : iConfig.getParameterNames()) {
      if (iConfig.existsAs<bool>(param))
        params.set(param, iConfig.getParameter<bool>(param));
      if (iConfig.existsAs<int>(param))
        params.set(param, iConfig.getParameter<int>(param));
      if (iConfig.existsAs<unsigned>(param))
        params.set<unsigned long long>(param, iConfig.getParameter<unsigned>(param));
      if (iConfig.existsAs<double>(param))
        params.set(param, iConfig.getParameter<double>(param));
      if (iConfig.existsAs<std::string>(param))
        params.set(param, iConfig.getParameter<std::string>(param));
      if (iConfig.existsAs<std::vector<double> >(param))
        params.set(param, iConfig.getParameter<std::vector<double> >(param));
      if (iConfig.existsAs<edm::ParameterSet>(param))
        params.set(param, fromParameterSet(iConfig.getParameter<edm::ParameterSet>(param)));
    }
    return params;
  }
}  // namespace cepgen

#endif
