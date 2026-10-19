/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "CommonTools/RecoAlgos/interface/DetSetVectorFlattener.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

using TotemRPRecHitFlattener = converter::helper::DetSetVectorFlattener<TotemRPRecHit, TotemRPDetId>;
DEFINE_FWK_MODULE(TotemRPRecHitFlattener);
using TotemRPUVPatternFlattener = converter::helper::DetSetVectorFlattener<TotemRPUVPattern, TotemRPDetId>;
DEFINE_FWK_MODULE(TotemRPUVPatternFlattener);
using TotemRPLocalTrackFlattener = converter::helper::DetSetVectorFlattener<TotemRPLocalTrack, TotemRPDetId>;
DEFINE_FWK_MODULE(TotemRPLocalTrackFlattener);

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

using CTPPSDiamondRecHitFlattener = converter::helper::DetSetVectorFlattener<CTPPSDiamondRecHit, CTPPSDiamondDetId>;
DEFINE_FWK_MODULE(CTPPSDiamondRecHitFlattener);
using CTPPSDiamondLocalTrackFlattener =
    converter::helper::DetSetVectorFlattener<CTPPSDiamondLocalTrack, CTPPSDiamondDetId>;
DEFINE_FWK_MODULE(CTPPSDiamondLocalTrackFlattener);

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

using CTPPSPixelRecHitFlattener = converter::helper::DetSetVectorFlattener<CTPPSPixelRecHit, CTPPSPixelDetId>;
DEFINE_FWK_MODULE(CTPPSPixelRecHitFlattener);
using CTPPSPixelLocalTrackFlattener = converter::helper::DetSetVectorFlattener<CTPPSPixelLocalTrack, CTPPSPixelDetId>;
DEFINE_FWK_MODULE(CTPPSPixelLocalTrackFlattener);
