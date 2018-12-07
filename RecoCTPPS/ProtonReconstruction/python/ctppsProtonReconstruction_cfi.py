import FWCore.ParameterSet.Config as cms

ctppsProtonReconstruction = cms.EDProducer('CTPPSProtonReconstruction',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    doSingleRPReconstruction = cms.bool(True),
    doMultiRPReconstruction = cms.bool(True),

    fitVtxY = cms.bool(True),
)
