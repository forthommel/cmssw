#ifndef SimTotem_TotemT2Producer_TotemT2DigitiserAlgorithm_h
#define SimTotem_TotemT2Producer_TotemT2DigitiserAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/ForwardGeometry/interface/TotemGeometry.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

class TotemT2DigitiserAlgorithm {
public:
  explicit TotemT2DigitiserAlgorithm(const edm::ParameterSet&);
  virtual ~TotemT2DigitiserAlgorithm();

  void setRunConditions(const TotemGeometry&);

  void addHits(const std::vector<PCaloHit>&, int bx);
  virtual void run(edmNew::DetSetVector<TotemT2Digi>&) = 0;
  void reset();

private:
  const TotemGeometry* geom_;
  std::map<int, std::vector<PCaloHit> > hits_vs_bx_;
};

#endif
