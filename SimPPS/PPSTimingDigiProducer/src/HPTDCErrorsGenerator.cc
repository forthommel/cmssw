/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "SimPPS/PPSTimingDigiProducer/interface/HPTDCErrorsGenerator.h"

#include <CLHEP/Random/RandFlat.h>

HPTDCErrorsGenerator::HPTDCErrorsGenerator(const edm::ParameterSet& iConfig)
    : internalFatalChipError_(iConfig.getParameter<double>("internalFatalChipError")),
      eventLost_(iConfig.getParameter<double>("eventLost")),
      hitRejectedByEventSizeLimit_(iConfig.getParameter<double>("hitRejectedByEventSizeLimit")),
      hitErrorGroup3_(iConfig.getParameter<double>("hitErrorGroup3")),
      hitLostL1OverflowGroup3_(iConfig.getParameter<double>("hitLostL1OverflowGroup3")),
      hitLostROFifoOverflowGroup3_(iConfig.getParameter<double>("hitLostROFifoOverflowGroup3")),
      hitErrorGroup2_(iConfig.getParameter<double>("hitErrorGroup2")),
      hitLostL1OverflowGroup2_(iConfig.getParameter<double>("hitLostL1OverflowGroup2")),
      hitLostROFifoOverflowGroup2_(iConfig.getParameter<double>("hitLostROFifoOverflowGroup2")),
      hitErrorGroup1_(iConfig.getParameter<double>("hitErrorGroup1")),
      hitLostL1OverflowGroup1_(iConfig.getParameter<double>("hitLostL1OverflowGroup1")),
      hitLostROFifoOverflowGroup1_(iConfig.getParameter<double>("hitLostROFifoOverflowGroup1")),
      hitErrorGroup0_(iConfig.getParameter<double>("hitErrorGroup0")),
      hitLostL1OverflowGroup0_(iConfig.getParameter<double>("hitLostL1OverflowGroup0")),
      hitLostROFifoOverflowGroup0_(iConfig.getParameter<double>("hitLostROFifoOverflowGroup0")) {}

HPTDCErrorFlags HPTDCErrorsGenerator::generate(CLHEP::HepRandomEngine* rnd) const {
  HPTDCErrorFlags flags;
  flags.setInternalFatalChipError(CLHEP::RandFlat::shoot(rnd) < internalFatalChipError_);
  flags.setEventLost(CLHEP::RandFlat::shoot(rnd) < eventLost_);
  flags.setHitRejectedByEventSizeLimit(CLHEP::RandFlat::shoot(rnd) < hitRejectedByEventSizeLimit_);
  flags.setHitErrorGroup3(CLHEP::RandFlat::shoot(rnd) < hitErrorGroup3_);
  flags.setHitLostL1OverflowGroup3(CLHEP::RandFlat::shoot(rnd) < hitLostL1OverflowGroup3_);
  flags.setHitLostROFifoOverflowGroup3(CLHEP::RandFlat::shoot(rnd) < hitLostROFifoOverflowGroup3_);
  flags.setHitErrorGroup2(CLHEP::RandFlat::shoot(rnd) < hitErrorGroup2_);
  flags.setHitLostL1OverflowGroup2(CLHEP::RandFlat::shoot(rnd) < hitLostL1OverflowGroup2_);
  flags.setHitLostROFifoOverflowGroup2(CLHEP::RandFlat::shoot(rnd) < hitLostROFifoOverflowGroup2_);
  flags.setHitErrorGroup1(CLHEP::RandFlat::shoot(rnd) < hitErrorGroup1_);
  flags.setHitLostL1OverflowGroup1(CLHEP::RandFlat::shoot(rnd) < hitLostL1OverflowGroup1_);
  flags.setHitLostROFifoOverflowGroup1(CLHEP::RandFlat::shoot(rnd) < hitLostROFifoOverflowGroup1_);
  flags.setHitErrorGroup0(CLHEP::RandFlat::shoot(rnd) < hitErrorGroup0_);
  flags.setHitLostL1OverflowGroup0(CLHEP::RandFlat::shoot(rnd) < hitLostL1OverflowGroup0_);
  flags.setHitLostROFifoOverflowGroup0(CLHEP::RandFlat::shoot(rnd) < hitLostROFifoOverflowGroup0_);
  return flags;
}

edm::ParameterSetDescription HPTDCErrorsGenerator::description() {
  edm::ParameterSetDescription hptdc_err_desc;
  hptdc_err_desc.add<double>("internalFatalChipError", 0.);
  hptdc_err_desc.add<double>("eventLost", 0.);
  hptdc_err_desc.add<double>("hitRejectedByEventSizeLimit", 0.);
  hptdc_err_desc.add<double>("hitErrorGroup3", 0.);
  hptdc_err_desc.add<double>("hitLostL1OverflowGroup3", 0.);
  hptdc_err_desc.add<double>("hitLostROFifoOverflowGroup3", 0.);
  hptdc_err_desc.add<double>("hitErrorGroup2", 0.);
  hptdc_err_desc.add<double>("hitLostL1OverflowGroup2", 0.);
  hptdc_err_desc.add<double>("hitLostROFifoOverflowGroup2", 0.);
  hptdc_err_desc.add<double>("hitErrorGroup1", 0.);
  hptdc_err_desc.add<double>("hitLostL1OverflowGroup1", 0.);
  hptdc_err_desc.add<double>("hitLostROFifoOverflowGroup1", 0.);
  hptdc_err_desc.add<double>("hitErrorGroup0", 0.);
  hptdc_err_desc.add<double>("hitLostL1OverflowGroup0", 0.);
  hptdc_err_desc.add<double>("hitLostROFifoOverflowGroup0", 0.);
  return hptdc_err_desc;
}
