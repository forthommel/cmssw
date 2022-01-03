import FWCore.ParameterSet.Config as cms
from CalibPPS.ESProducers.ctppsBeamParametersFromLHCInfoESSource_cfi import ctppsBeamParametersFromLHCInfoESSource as _esLHCinfo
from SimPPS.DirectSimProducer.ppsDirectProtonSimulation_cfi import ppsDirectProtonSimulation as _dirProtonSim
from IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi import beamDivergenceVtxGenerator as _vtxGen

# vertex smearing
beamDivergenceVtxGenerator = _vtxGen.clone()

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

    # vertex sigma (cm) - do not apply vertex smearing again
    vtxStddevX = cms.double(0.0),
    vtxStddevY = cms.double(0.0),
    vtxStddevZ = cms.double(0.0)
)

# direct simulation
ppsDirectProtonSimulation = _dirProtonSim.clone(
    hepMCTag = cms.InputTag('beamDivergenceVtxGenerator'),
    pitchStrips = cms.double(66.e-3 * 12 / 19), # effective value to reproduce real RP resolution
    pitchPixelsHor = cms.double(5.e-3),
    pitchPixelsVer = cms.double(80.e-3),
    produceScoringPlaneHits = cms.bool(False),
)

#Update vertex depending on CMS vertex shift
from Configuration.Eras.Modifier_ctpps_2017_cff import ctpps_2017
from Configuration.Eras.Modifier_ctpps_2018_cff import ctpps_2018
#from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import Realistic25ns13TeVEarly2017CollisionVtxSmearingParameters as vtxOffset2017
#from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import Realistic25ns13TeVEarly2018CollisionVtxSmearingParameters as vtxOffset2018
ctpps_2017.toModify(ctppsBeamParametersFromLHCInfoESSource, 
		vtxOffsetX45=cms.double(+0.024755),
		vtxOffsetY45=cms.double(-0.069233),
		vtxOffsetZ45=cms.double(-0.820540)
)
ctpps_2018.toModify(ctppsBeamParametersFromLHCInfoESSource, 
		vtxOffsetX45=cms.double(-0.0107682),
		vtxOffsetY45=cms.double(-0.041722),
		vtxOffsetZ45=cms.double(-0.035748)
)