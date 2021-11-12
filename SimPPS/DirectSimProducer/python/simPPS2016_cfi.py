from CalibPPS.ESProducers.ctppsCompositeESSource_cfi import ctppsCompositeESSource as _esComp
from CalibPPS.ESProducers.ppsAssociationCutsESSource_cfi import ppsAssociationCutsESSource as _esAssCuts
from CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff import use_single_infinite_iov_entry, p2016
from Geometry.VeryForwardGeometry.commons_cff import cloneGeometry
from SimPPS.DirectSimProducer.profile_2016_preTS2_cff import profile_2016_preTS2
from SimPPS.DirectSimProducer.profile_2016_postTS2_cff import profile_2016_postTS2

ppsAssociationCutsESSource = _esAssCuts.clone()
use_single_infinite_iov_entry(ppsAssociationCutsESSource, p2016)
XMLIdealGeometryESSource_CTPPS, ctppsGeometryESModule = cloneGeometry('Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi')

ctppsCompositeESSource = _esComp.clone(
    generateEveryNEvents = 100,
    periods = [profile_2016_postTS2, profile_2016_preTS2],
    # geometry (using 2017 here is OK)
    compactViewTag = ctppsGeometryESModule.compactViewTag,
    isRun2 = ctppsGeometryESModule.isRun2
)

def enableSPReco(process):
    # local reconstruction (if scoring plane hits stored)
    process.load('RecoPPS.Local.totemRPLocalReconstruction_cff')
    process.load('RecoPPS.Local.ctppsLocalTrackLiteProducer_cff')
    process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsLocalTrackLiteProducer.includeStrips = True
    process.ctppsLocalTrackLiteProducer.includePixels = False
    process.ctppsLocalTrackLiteProducer.includeDiamonds = False
    process.reco_local = cms.Sequence(
        process.totemRPUVPatternFinder
        * process.totemRPLocalTrackFitter
        * process.ctppsLocalTrackLiteProducer
    )
)
