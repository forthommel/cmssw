
/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DQM/CTPPS/interface/TotemT2Segmentation.h"
#include "FWCore/Utilities/interface/Exception.h"

TotemT2Segmentation::TotemT2Segmentation(TH2D&& base_hist) : TH2D(base_hist) {
  const auto nx = TH2D::GetXaxis()->GetNbins(), ny = TH2D::GetYaxis()->GetNbins();
  for (unsigned short arm = 0; arm <= CTPPSDetId::maxArm; ++arm)
    for (unsigned short plane = 0; plane <= TotemT2DetId::maxPlane; ++plane)
      for (unsigned short id = 0; id <= TotemT2DetId::maxChannel; ++id) {
        const TotemT2DetId detid(arm, plane, id);
        bins_map_[detid] = computeBins(nx, ny, detid);
      }
}

void TotemT2Segmentation::fill(const TotemT2DetId& detid, double value) {
  if (bins_map_.count(detid) == 0)
    throw cms::Exception("TotemT2Segmentation") << "Failed to retrieve list of bins for TotemT2DetId " << detid << ".";
  for (const auto& bin : bins_map_.at(detid))
    TH2D::Fill(bin.first, bin.second, value);
}

std::vector<std::pair<short, short> > TotemT2Segmentation::computeBins(int nbinsx,
                                                                       int nbinsy,
                                                                       const TotemT2DetId& detid) const {
  std::vector<std::pair<short, short> > bins;
  // find the histogram centre
  const auto ox = ceil(nbinsx * 0.5), oy = ceil(nbinsy * 0.5);
  const auto ax = floor(nbinsx * 0.5), by = floor(nbinsy * 0.5);

  if (detid.plane() % 2 == 0) {  // even planes => XXX
    for (int ix = 0; ix < nbinsx; ++ix)
      for (int iy = 0; iy < nbinsy; ++iy)
        //if (...)
        bins.emplace_back(ix, iy);
  } else {  // odd planes => XXX
    for (int ix = 0; ix < nbinsx; ++ix)
      for (int iy = 0; iy < nbinsy; ++iy)
        //if (...)
        bins.emplace_back(ix, iy);
  }

  return bins;
}
