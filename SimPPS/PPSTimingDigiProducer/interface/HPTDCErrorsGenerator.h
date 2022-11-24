/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef SimPPS_PPSTimingDigiProducer_HPTDCErrorsGenerator_h
#define SimPPS_PPSTimingDigiProducer_HPTDCErrorsGenerator_h

#include "DataFormats/CTPPSDigi/interface/HPTDCErrorFlags.h"

namespace edm {
  class ParameterSet;
  class ParameterSetDescription;
}  // namespace edm

namespace CLHEP {
  class HepRandomEngine;
}

/// A HPTDCErrorFlags generator given a flat error probability distribution
class HPTDCErrorsGenerator {
public:
  explicit HPTDCErrorsGenerator(const edm::ParameterSet&);

  HPTDCErrorFlags generate(CLHEP::HepRandomEngine*) const;

  static edm::ParameterSetDescription description();

private:
  double internalFatalChipError_;
  double eventLost_;
  double hitRejectedByEventSizeLimit_;
  double hitErrorGroup3_;
  double hitLostL1OverflowGroup3_;
  double hitLostROFifoOverflowGroup3_;
  double hitErrorGroup2_;
  double hitLostL1OverflowGroup2_;
  double hitLostROFifoOverflowGroup2_;
  double hitErrorGroup1_;
  double hitLostL1OverflowGroup1_;
  double hitLostROFifoOverflowGroup1_;
  double hitErrorGroup0_;
  double hitLostL1OverflowGroup0_;
  double hitLostROFifoOverflowGroup0_;
};

#endif
