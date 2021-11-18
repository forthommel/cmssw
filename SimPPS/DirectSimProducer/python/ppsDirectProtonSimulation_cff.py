import FWCore.ParameterSet.Config as cms
from CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi import ctppsBeamParametersFromLHCInfoESSource as _esLHCinfo
from SimPPS.DirectSimProducer.ppsDirectProtonSimulation_cfi import ppsDirectProtonSimulation as _dirProtonSim

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

# direct simulation
ppsDirectProtonSimulation = _dirProtonSim.clone(
    hepMCTag = cms.InputTag('beamDivergenceVtxGenerator'),
    pitchStrips = cms.double(66.e-3 * 12 / 19), # effective value to reproduce real RP resolution
    pitchPixelsHor = cms.double(5.e-3),
    pitchPixelsVer = cms.double(80.e-3),
    produceScoringPlaneHits = cms.bool(False),
)
