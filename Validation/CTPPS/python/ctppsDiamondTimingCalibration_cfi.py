import FWCore.ParameterSet.Config as cms

ctppsDiamondTimingCalibration = cms.EDAnalyzer('CTPPSDiamondTimingCalibration',
    diamondRecHitsTag = cms.InputTag('ctppsDiamondRecHits'),
    diamondLocalTracksTag = cms.InputTag('ctppsDiamondLocalTracks'),
    OOTindex = cms.int32(3),
)
