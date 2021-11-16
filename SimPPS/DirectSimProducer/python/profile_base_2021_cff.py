import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base as _base
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2021 as selected_optics

# base profile settings for 2021
profile_base_2021 = _base.clone(
    ctppsLHCInfo = _base.ctppsLHCInfo.clone(
        beamEnergy = 7000
    ),
    ctppsOpticalFunctions = _base.ctppsOpticalFunctions.clone(
        opticalFunctions = selected_optics.opticalFunctions,
        scoringPlanes = selected_optics.scoringPlanes,
    ),
    ctppsDirectSimuData = _base.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("1E3*([xi] - 0.20)"),
        empiricalAperture56 = cms.string("1E3*([xi] - 0.20)")
    )
)
