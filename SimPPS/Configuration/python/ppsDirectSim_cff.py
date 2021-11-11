# configuration for composite source of alignment, optics, ...
from CalibPPS.ESProducers.ctppsCompositeESSource_cfi import ctppsCompositeESSource as _esComp
from CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi import ctppsBeamParametersFromLHCInfoESSource as _esLHCinfo
from CalibPPS.ESProducers.ppsAssociationCutsESSource_cfi import ppsAssociationCutsESSource as _esAssCuts
from CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff import use_single_infinite_iov_entry
# direct proton simulation
from Validation.CTPPS.ctppsDirectProtonSimulation_cfi import ctppsDirectProtonSimulation as _dirProtonSim

ctppsCompositeESSource = _esComp.clone(
    generateEveryNEvents = 100,
)
# beam parameters as determined by PPS
ctppsBeamParametersFromLHCInfoESSource = _esLHCinfo.clone(
    lhcInfoLabel = cms.string(""),
    # beam divergence (rad)
    beamDivX45 = cms.double(30.e-6),
    beamDivX56 = cms.double(30.e-6),
    beamDivY45 = cms.double(30.e-6),
    beamDivY56 = cms.double(30.e-6),
    # vertex offset (cm)
    vtxOffsetX45 = cms.double(0.),
    vtxOffsetX56 = cms.double(0.),
    vtxOffsetY45 = cms.double(0.),
    vtxOffsetY56 = cms.double(0.),
    vtxOffsetZ45 = cms.double(0.),
    vtxOffsetZ56 = cms.double(0.),
    # vertex sigma (cm)
    vtxStddevX = cms.double(1.e-3),
    vtxStddevY = cms.double(1.e-3),
    vtxStddevZ = cms.double(5.)
)

ppsAssociationCutsESSource = _esAssCuts.clone()
use_single_infinite_iov_entry(ppsAssociationCutsESSource, ac.p2016) #FIXME

# direct simulation
ctppsDirectProtonSimulation = _dirProtonSim.clone(
    hepMCTag = cms.InputTag('beamDivergenceVtxGenerator'),
    pitchStrips = cms.double(66.e-3 * 12 / 19), # effective value to reproduce real RP resolution
    pitchPixelsHor = cms.double(5.e-3),
    pitchPixelsVer = cms.double(80.e-3),
    produceScoringPlaneHits = cms.bool(False),
)

