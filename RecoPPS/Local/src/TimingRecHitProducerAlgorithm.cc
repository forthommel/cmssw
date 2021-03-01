/****************************************************************************
*
* This is a part of PPS offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#include "RecoPPS/Local/interface/TimingRecHitProducerAlgorithm.h"

//----------------------------------------------------------------------------------------------------

template <typename D, typename R>
TimingRecHitProducerAlgorithm<D, R>::TimingRecHitProducerAlgorithm(const edm::ParameterSet& iConfig)
    : ts_to_ns_(iConfig.getParameter<double>("timeSliceNs")),
      apply_calib_(iConfig.getParameter<bool>("applyCalibration")) {}

template <typename D, typename R>
void TimingRecHitProducerAlgorithm<D, R>::setCalibration(const PPSTimingCalibration& calib,
                                                         const PPSTimingCalibrationLUT& calibLUT) {
  calib_ = calib;
  calibLUT_ = calibLUT;
  calib_fct_ = std::make_unique<reco::FormulaEvaluator>(calib_.formula());
}
