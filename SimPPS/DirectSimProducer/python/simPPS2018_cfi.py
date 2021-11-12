from CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff import use_single_infinite_iov_entry, p2018
from CalibPPS.ESProducers.ppsAssociationCutsESSource_cfi import ppsAssociationCutsESSource as _esAssCuts
from Geometry.VeryForwardGeometry.commons_cff import cloneGeometry
from SimPPS.DirectSimProducer.profile_2018_preTS1_cff import profile_2018_preTS1
from SimPPS.DirectSimProducer.profile_2018_postTS2_cff import profile_2018_postTS2
from SimPPS.DirectSimProducer.profile_2018_TS1_TS2_cff import profile_2018_TS1_TS2

ppsAssociationCutsESSource = _esAssCuts.clone()
use_single_infinite_iov_entry(ppsAssociationCutsESSource, p2018)
XMLIdealGeometryESSource_CTPPS, ctppsGeometryESModule = cloneGeometry('Geometry.VeryForwardGeometry.geometryRPFromDD_2018_cfi')

ctppsCompositeESSource = _esComp.clone(
    generateEveryNEvents = 100,
    periods = [profile_2018_postTS2, profile_2018_preTS1, profile_2018_TS1_TS2],
    compactViewTag = ctppsGeometryESModule.compactViewTag,
    isRun2 = ctppsGeometryESModule.isRun2
)

def enableSPReco(process):
    # local reconstruction (if scoring plane hits stored)
    process.load('RecoPPS.Local.ctppsPixelLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsDiamondLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsLocalTrackLiteProducer_cff')
    process.ctppsPixelLocalTracks.tag = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsDiamondLocalTracks.recHitsTag = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsLocalTrackLiteProducer.includeStrips = False
    process.ctppsLocalTrackLiteProducer.includePixels = True
    process.ctppsLocalTrackLiteProducer.includeDiamonds = True
    process.reco_local = cms.Sequence(
        process.ctppsPixelLocalTracks
        * process.ctppsDiamondLocalReconstruction
        * process.ctppsLocalTrackLiteProducer
    )
)
