import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("PPXZGenerator",
  verbosity = cms.untracked.uint32(0),
  debug = cms.untracked.uint32(0),

  m_X = cms.double(1200),
  m_Z = cms.double(91),

  p_beam = cms.double(6500),
  m_XZ_min = cms.double(1300),
  c_XZ = cms.double(0.04),

  p_z_LAB_2p_mean = cms.double(180),
  p_z_LAB_2p_sigma = cms.double(450)
)
