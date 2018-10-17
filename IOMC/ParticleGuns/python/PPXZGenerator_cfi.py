import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("PPXZGenerator",
  verbosity = cms.untracked.uint32(0),
  debug = cms.untracked.uint32(0),

  decayZToElectrons = cms.bool(True),
  decayZToMuons = cms.bool(False),

  m_X = cms.double(1200),

  m_Z_mean = cms.double(91.1876),
  m_Z_gamma = cms.double(2.4952),

  m_e = cms.double(0.5109e-3),
  m_mu = cms.double(105.658e-3),

  p_beam = cms.double(6500),

  m_XZ_min = cms.double(1300),
  c_XZ = cms.double(0.04),

  p_z_LAB_2p_mean = cms.double(180),
  p_z_LAB_2p_sigma = cms.double(450)
)
