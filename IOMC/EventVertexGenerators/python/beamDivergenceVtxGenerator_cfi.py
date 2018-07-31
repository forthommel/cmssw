import FWCore.ParameterSet.Config as cms

beamDivergenceVtxGenerator = cms.EDProducer("BeamDivergenceVtxGenerator",
  src = cms.InputTag(""),

  simulateBeamDivergence = cms.bool(True),
  simulateVertex = cms.bool(True),

  # values in rad
  beamDivergenceX = cms.double(0E-6),
  beamDivergenceY = cms.double(0E-6),

  # values in cm
  vertexMeanX = cms.double(0),
  vertexMeanY = cms.double(0),
  vertexMeanZ = cms.double(0),

  vertexSigmaX = cms.double(0),
  vertexSigmaY = cms.double(0),
  vertexSigmaZ = cms.double(0),
)
