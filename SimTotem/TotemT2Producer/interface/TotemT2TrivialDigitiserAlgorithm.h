#ifndef SimTotem_TotemT2Producer_TotemT2TrivialDigitiserAlgorithm_h
#define SimTotem_TotemT2Producer_TotemT2TrivialDigitiserAlgorithm_h

#include "SimTotem/TotemT2Producer/interface/TotemT2DigitiserAlgorithm.h"
#include "CommonTools/Utils/interface/FormulaEvaluator.h"

class TotemT2TrivialDigitiserAlgorithm : public TotemT2DigitiserAlgorithm {
public:
  TotemT2TrivialDigitiserAlgorithm(const edm::ParameterSet&);

  void run(edmNew::DetSetVector<TotemT2Digi>&) override;

private:
  std::unique_ptr<reco::FormulaEvaluator> tf_;  ///< Q-to-timing transfer function
};

#endif
