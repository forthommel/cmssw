import FWCore.ParameterSet.Config as cms

from SimPPS.DirectSimProducer.base_cff import *
import CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff as ac
ac.use_single_infinite_iov_entry(ac.ppsAssociationCutsESSource, ac.p2016)
ppsAssociationCutsESSource = ac.ppsAssociationCutsESSource

# base profile settings for 2016
profile_base_2016 = profile_base.clone(
  ctppsLHCInfo = dict(
    beamEnergy = 6500
  )
)

# RP ids
rpIds = cms.PSet(
  rp_45_F = cms.uint32(3),
  rp_45_N = cms.uint32(2),
  rp_56_N = cms.uint32(102),
  rp_56_F = cms.uint32(103)
)
