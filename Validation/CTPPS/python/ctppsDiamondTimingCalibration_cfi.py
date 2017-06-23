import FWCore.ParameterSet.Config as cms

ctppsDiamondTimingCalibration = cms.EDAnalyzer('CTPPSDiamondTimingCalibration',
    record = cms.string('CTPPSTimingCalibrationRcd'),
    loggingOn = cms.untracked.bool(True),
    SinceAppendMode = cms.bool(True),
    Source = cms.PSet(
        IOVRun=cms.untracked.uint32(1)
    ),
    diamondRecHitsTag = cms.InputTag('ctppsDiamondRecHits'),
    diamondLocalTracksTag = cms.InputTag('ctppsDiamondLocalTracks'),
    OOTindex = cms.int32(3),
)
