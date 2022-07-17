
/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DQM/CTPPS/interface/TotemT2Segmentation.h"
#include "FWCore/Utilities/interface/Exception.h"

TotemT2Segmentation::TotemT2Segmentation(TH2D&& base_hist) : TH2D(base_hist) {}

void TotemT2Segmentation::fill(const TotemT2DetId& detid, double value) {
  if (bins_map_.count(detid) == 0)
    throw cms::Exception("TotemT2Segmentation") << "Failed to retrieve list of bins for TotemT2DetId " << detid << ".";
  for (const auto& bin : bins_map_.at(detid))
    TH2D::Fill(bin.first, bin.second, value);
}
