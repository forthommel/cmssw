import FWCore.ParameterSet.Config as cms

from RecoPPS.Local.ctppsDiamondRecHitFlattener_cfi import ctppsDiamondRecHitFlattener as _diamond_rechits
from RecoPPS.Local.ctppsDiamondLocalTrackFlattener_cfi import ctppsDiamondLocalTrackFlattener as _diamond_tracks
from RecoPPS.Local.ctppsPixelRecHitFlattener_cfi import ctppsPixelRecHitFlattener as _pixel_rechits
from RecoPPS.Local.ctppsPixelLocalTrackFlattener_cfi import ctppsPixelLocalTrackFlattener as _pixel_tracks


ctppsDiamondRecHitFlattener = _diamond_rechits.clone(input = cms.InputTag("ctppsDiamondRecHits"))
ctppsDiamondLocalTrackFlattener = _diamond_tracks.clone(input = cms.InputTag("ctppsDiamondLocalTracks"))
ctppsPixelRecHitFlattener = _pixel_rechits.clone(input = "ctppsPixelRecHits",)
ctppsPixelLocalTrackFlattener = _pixel_tracks.clone(input = "ctppsPixelLocalTracks",)

ctppsFlattenerTask = cms.Task(
    ctppsDiamondRecHitFlattener,
    ctppsDiamondLocalTrackFlattener,
    ctppsPixelRecHitFlattener,
    ctppsPixelLocalTrackFlattener
)
ctppsFlattener = cms.Sequence(ctppsFlattenerTask)
