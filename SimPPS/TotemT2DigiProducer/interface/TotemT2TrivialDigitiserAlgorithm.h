#ifndef SimPPS_TotemT2DigiProducer_TotemT2TrivialDigitiserAlgorithm_h
#define SimPPS_TotemT2DigiProducer_TotemT2TrivialDigitiserAlgorithm_h

#include "SimPPS/TotemT2DigiProducer/interface/TotemT2DigitiserAlgorithm.h"
#include "CommonTools/Utils/interface/FormulaEvaluator.h"

class TotemT2TrivialDigitiserAlgorithm : public TotemT2DigitiserAlgorithm {
public:
  TotemT2TrivialDigitiserAlgorithm(const edm::ParameterSet&);

  void run(edm::DetSetVector<TotemT2Digi>&) override;

private:
  std::unique_ptr<reco::FormulaEvaluator> tf_; ///< Q-to-timing transfer function
};

#endif
