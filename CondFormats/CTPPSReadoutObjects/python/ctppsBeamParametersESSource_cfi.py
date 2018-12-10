import FWCore.ParameterSet.Config as cms

ctppsBeamParametersESSource = cms.ESSource("CTPPSBeamParametersESSource",
  setBeamPars = cms.untracked.bool(True),

  #  Beam momentum  (GeV)
  beamMom45 = cms.double(6500.),
  beamMom56 = cms.double(6500.),

  #  Beta*  (cm)
  betaStarX45 = cms.double(30.),
  betaStarX56 = cms.double(30.),
  betaStarY45 = cms.double(30.),
  betaStarY56 = cms.double(30.),

  #  Beam divergence  (rad)
  beamDivX45 = cms.double(0.1),
  beamDivX56 = cms.double(0.1),
  beamDivY45 = cms.double(0.1),
  beamDivY56 = cms.double(0.1),

  #  Half crossing angle  (rad)
  halfXangleX45 = cms.double(80e-6),
  halfXangleX56 = cms.double(80e-6),
  halfXangleY45 = cms.double(80e-6),
  halfXangleY56 = cms.double(80e-6),

  #  Vertex offset  (cm)
  vtxOffsetX45 = cms.double(0.01),
  vtxOffsetX56 = cms.double(0.01),
  vtxOffsetY45 = cms.double(0.01),
  vtxOffsetY56 = cms.double(0.01),
  vtxOffsetZ45 = cms.double(0.01),
  vtxOffsetZ56 = cms.double(0.01),

  #  Vertex sigma  (cm)
  vtxStddevX = cms.double(0.02),
  vtxStddevY = cms.double(0.02),
  vtxStddevZ = cms.double(0.02)
)
