from SimPPS.DirectSimProducer.profile_base_cff import profile_base as _base
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2017 as selected_optics

# base profile settings for 2017
profile_base_2017 = _base.clone(
    ctppsLHCInfo = _base.ctppsLHCInfo.clone(
        beamEnergy = 6500
    ),
    ctppsOpticalFunctions = _base.ctppsOpticalFunctions.clone(
        opticalFunctions = selected_optics.opticalFunctions,
        scoringPlanes = selected_optics.scoringPlanes,
    )
)
