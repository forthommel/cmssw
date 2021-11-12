import FWCore.ParameterSet.Config as cms

from SimPPS.DirectSimProducer.base_cff import *
import CalibPPS.ESProducers.ppsAssociationCuts_non_DB_cff as ac
ac.use_single_infinite_iov_entry(ac.ppsAssociationCutsESSource, ac.p2017)
ppsAssociationCutsESSource = ac.ppsAssociationCutsESSource

from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2017 as selected_optics

# base profile settings for 2017
profile_base_2017 = profile_base.clone(
  ctppsLHCInfo = dict(
    beamEnergy = 6500
  ),

  ctppsOpticalFunctions = dict(
    opticalFunctions = selected_optics.opticalFunctions,
    scoringPlanes = selected_optics.scoringPlanes,
  )
)

# RP ids
rpIds = cms.PSet(
  rp_45_F = cms.uint32(23),
  rp_45_N = cms.uint32(3),
  rp_56_N = cms.uint32(103),
  rp_56_F = cms.uint32(123)
)
