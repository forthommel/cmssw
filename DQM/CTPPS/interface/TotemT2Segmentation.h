/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef DQM_CTPPS_TotemT2Segmentation_h
#define DQM_CTPPS_TotemT2Segmentation_h

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"

#include <unordered_map>

#include "TH2.h"

class TotemT2Segmentation : public TH2D {
public:
  TotemT2Segmentation(TH2D&&);

  void fill(const TotemT2DetId&, double value = 1.);

private:
  std::unordered_map<TotemT2DetId, std::vector<std::pair<short, short> > > bins_map_;
};

#endif
