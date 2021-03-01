/****************************************************************************
*
* This is a part of PPS offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#include "RecoPPS/Local/interface/TotemT2RecHitProducerAlgorithm.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"

void TotemT2RecHitProducerAlgorithm::build(const CTPPSGeometry& geom,
                                           const edm::DetSetVector<TotemT2Digi>& input,
                                           edm::DetSetVector<TotemT2RecHit>& output) {
  for (const auto& vec : input) {
    const TotemT2DetId detid(vec.detId());
  }
}
