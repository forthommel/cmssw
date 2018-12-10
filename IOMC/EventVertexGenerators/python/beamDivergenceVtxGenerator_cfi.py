import FWCore.ParameterSet.Config as cms

beamDivergenceVtxGenerator = cms.EDProducer("BeamDivergenceVtxGenerator",
  src = cms.InputTag("generator", "unsmeared"),

  simulateBeamDivergence = cms.bool(True),
  simulateVertex = cms.bool(True)
)
