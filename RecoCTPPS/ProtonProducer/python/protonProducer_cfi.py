import FWCore.ParameterSet.Config as cms

protons = cms.EDProducer('ProtonProducer',
    protonTag = cms.InputTag('totemRPLocalTrackFitter'),
    useXiInterpolation = cms.bool(True),
    xiInterpolationFile = cms.FileInPath('RecoCTPPS/ProtonProducer/data/ctpps_optics_v1.root'),
)
