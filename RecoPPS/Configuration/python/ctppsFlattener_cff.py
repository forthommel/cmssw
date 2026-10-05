import FWCore.ParameterSet.Config as cms

from RecoPPS.Local.totemRPRecHitFlattener_cfi import totemRPRecHitFlattener as _totemrp_rechits
from RecoPPS.Local.totemRPUVPatternFlattener_cfi import totemRPUVPatternFlattener as _totemrp_uvpatterns
from RecoPPS.Local.totemRPLocalTrackFlattener_cfi import totemRPLocalTrackFlattener as _totemrp_tracks
from RecoPPS.Local.ctppsDiamondRecHitFlattener_cfi import ctppsDiamondRecHitFlattener as _diamond_rechits
from RecoPPS.Local.ctppsDiamondLocalTrackFlattener_cfi import ctppsDiamondLocalTrackFlattener as _diamond_tracks
from RecoPPS.Local.ctppsPixelRecHitFlattener_cfi import ctppsPixelRecHitFlattener as _pixel_rechits
from RecoPPS.Local.ctppsPixelLocalTrackFlattener_cfi import ctppsPixelLocalTrackFlattener as _pixel_tracks


totemRPRecHitFlattener = _totemrp_rechits.clone(input = cms.InputTag("totemRPRecHitProducer"))
totemRPUVPatternFlattener = _totemrp_uvpatterns.clone(input = cms.InputTag("totemRPUVPatternFinder"))
totemRPLocalTrackFlattener = _totemrp_tracks.clone(input = cms.InputTag("totemRPLocalTrackFitter"))
ctppsDiamondRecHitFlattener = _diamond_rechits.clone(input = cms.InputTag("ctppsDiamondRecHits"))
ctppsDiamondLocalTrackFlattener = _diamond_tracks.clone(input = cms.InputTag("ctppsDiamondLocalTracks"))
ctppsPixelRecHitFlattener = _pixel_rechits.clone(input = "ctppsPixelRecHits",)
ctppsPixelLocalTrackFlattener = _pixel_tracks.clone(input = "ctppsPixelLocalTracks",)

ctppsFlattenerTask = cms.Task(
    totemRPRecHitFlattener,
    totemRPUVPatternFlattener,
    totemRPLocalTrackFlattener,
    ctppsDiamondRecHitFlattener,
    ctppsDiamondLocalTrackFlattener,
    ctppsPixelRecHitFlattener,
    ctppsPixelLocalTrackFlattener
)
ctppsFlattener = cms.Sequence(ctppsFlattenerTask)
