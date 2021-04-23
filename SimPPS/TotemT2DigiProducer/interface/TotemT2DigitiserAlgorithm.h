#ifndef SimPPS_TotemT2DigiProducer_TotemT2DigitiserAlgorithm_h
#define SimPPS_TotemT2DigiProducer_TotemT2DigitiserAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

class TotemT2DigitiserAlgorithm {
public:
  explicit TotemT2DigitiserAlgorithm(const edm::ParameterSet&);
  virtual ~TotemT2DigitiserAlgorithm();

  void setRunConditions(const CTPPSGeometry&);

  void addHits(const std::vector<PCaloHit>&, int bx);
  virtual void run(edm::DetSetVector<TotemT2Digi>&) = 0;
  void reset();

private:
  const CTPPSGeometry* geom_;
  std::map<int, std::vector<PCaloHit> > hits_vs_bx_;
};

#endif
