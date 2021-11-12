from CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff import use_single_infinite_iov_entry, p2017
from CalibPPS.ESProducers.ppsAssociationCutsESSource_cfi import ppsAssociationCutsESSource as _esAssCuts
from Geometry.VeryForwardGeometry.commons_cff import cloneGeometry
from SimPPS.DirectSimProducer.profile_2017_preTS2_cff import profile_2017_preTS2
from SimPPS.DirectSimProducer.profile_2017_postTS2_cff import profile_2017_postTS2

ppsAssociationCutsESSource = _esAssCuts.clone()
use_single_infinite_iov_entry(ppsAssociationCutsESSource, p2017)
XMLIdealGeometryESSource_CTPPS, ctppsGeometryESModule = cloneGeometry('Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi')

ctppsCompositeESSource = _esComp.clone(
    generateEveryNEvents = 100,
    periods = [profile_2017_postTS2, profile_2017_preTS2],
    compactViewTag = ctppsGeometryESModule.compactViewTag,
    isRun2 = ctppsGeometryESModule.isRun2
)

def enableSPReco(process):
    # local reconstruction (if scoring plane hits stored)
    process.load('RecoPPS.Local.totemRPLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsPixelLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsDiamondLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsLocalTrackLiteProducer_cff')
    process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsPixelLocalTracks.tag = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsDiamondLocalTracks.recHitsTag = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsLocalTrackLiteProducer.includeStrips = True
    process.ctppsLocalTrackLiteProducer.includePixels = True
    process.ctppsLocalTrackLiteProducer.includeDiamonds = True
    process.reco_local = cms.Sequence(
        process.totemRPUVPatternFinder
        * process.totemRPLocalTrackFitter
        * process.ctppsPixelLocalTracks
        * process.ctppsDiamondLocalReconstruction
        * process.ctppsLocalTrackLiteProducer
    )
)
