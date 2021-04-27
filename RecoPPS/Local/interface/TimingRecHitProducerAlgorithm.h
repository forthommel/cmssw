/****************************************************************************
*
* This is a part of PPS-TOTEM offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#ifndef RecoPPS_Local_TimingRecHitProducerAlgorithm
#define RecoPPS_Local_TimingRecHitProducerAlgorithm

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/FormulaEvaluator.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "CondFormats/PPSObjects/interface/PPSTimingCalibration.h"

template <typename G, typename D, typename R>
class TimingRecHitProducerAlgorithm {
public:
  TimingRecHitProducerAlgorithm(const edm::ParameterSet& iConfig)
      : ts_to_ns_(iConfig.getParameter<double>("timeSliceNs")),
        apply_calib_(iConfig.getParameter<bool>("applyCalibration")) {}
  virtual ~TimingRecHitProducerAlgorithm() = default;

  void setCalibration(const PPSTimingCalibration& calib) {
    calib_ = calib;
    calib_fct_ = std::make_unique<reco::FormulaEvaluator>(calib_.formula());
  }
  virtual void build(const G&, const edm::DetSetVector<D>&, edm::DetSetVector<R>&) = 0;

protected:
  /// Conversion constant between time slice and absolute time (in ns)
  double ts_to_ns_;
  /// Switch on/off the timing calibration
  bool apply_calib_;
  /// DB-loaded calibration object
  PPSTimingCalibration calib_;
  /// Timing correction formula
  std::unique_ptr<reco::FormulaEvaluator> calib_fct_;
};

#endif
