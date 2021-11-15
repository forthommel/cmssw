from SimPPS.DirectSimProducer.profile_base_cff import profile_base as _base

# base profile settings for 2016
profile_base_2016 = _base.clone(
    ctppsLHCInfo = _base.ctppsLHCInfo.clone(
        beamEnergy = 6500
    )
)

