
/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *   Arkadiusz Cwikla
 *
 ****************************************************************************/

#include "DQM/CTPPS/interface/TotemT2Segmentation.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TH2D.h"

TotemT2Segmentation::TotemT2Segmentation(const TotemGeometry& geom, size_t nbinsx, size_t nbinsy)
    : geom_(geom), nbinsx_(nbinsx), nbinsy_(nbinsy) {
  for (unsigned short arm = 0; arm <= CTPPSDetId::maxArm; ++arm)
    for (unsigned short plane = 0; plane <= TotemT2DetId::maxPlane; ++plane)
      for (unsigned short id = 0; id <= TotemT2DetId::maxChannel; ++id) {
        const TotemT2DetId detid(arm, plane, id);
        bins_map_[detid] = computeBins(detid);
      }
}

void TotemT2Segmentation::fill(TH2D* hist, const TotemT2DetId& detid, double value) {
  if (bins_map_.count(detid) == 0)
    throw cms::Exception("TotemT2Segmentation") << "Failed to retrieve list of bins for TotemT2DetId " << detid << ".";
  if (static_cast<size_t>(hist->GetXaxis()->GetNbins()) != nbinsx_ ||
      static_cast<size_t>(hist->GetYaxis()->GetNbins()) != nbinsy_)
    throw cms::Exception("TotemT2Segmentation")
        << "Trying to fill a summary plot with invalid number of bins. "
        << "Should be of dimension (" << nbinsx_ << ", " << nbinsy_ << "), but has dimension ("
        << hist->GetXaxis()->GetNbins() << ", " << hist->GetYaxis()->GetNbins() << ").";
  for (const auto& bin : bins_map_.at(detid))
    hist->Fill(bin.first, bin.second, value);
}

std::vector<std::pair<short, short> > TotemT2Segmentation::computeBins(const TotemT2DetId& detid) const {
  std::vector<std::pair<short, short> > bins;
  // find the histogram centre
  const auto ox = ceil(nbinsx_ * 0.5), oy = ceil(nbinsy_ * 0.5);
  // compute the ellipse parameters
  const auto ax = floor(nbinsx_ * 0.5), by = floor(nbinsy_ * 0.5);

  //FIXME this is where magic happens...
  //
  // One should find a (geometric) way of associating a DetId to a vector<ix, iy> of bins
  // given the size (nx_, ny_) of the TH2D('s) to be filled

  // NOTE: the geometry object (geom_) can potentially be used as a helper

  if (detid.plane() % 2 == 0) {  // even planes => XXX
    for (size_t ix = 0; ix < nbinsx_; ++ix)
      for (size_t iy = 0; iy < nbinsy_; ++iy)
        //if (ix ... iy ...)
        bins.emplace_back(ix, iy);
  } else {  // odd planes => XXX
    for (size_t ix = 0; ix < nbinsx_; ++ix)
      for (size_t iy = 0; iy < nbinsy_; ++iy)
        //if (ix ... iy ...)
        bins.emplace_back(ix, iy);
  }

  return bins;
}
