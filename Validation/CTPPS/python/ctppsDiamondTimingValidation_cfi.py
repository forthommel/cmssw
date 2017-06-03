import FWCore.ParameterSet.Config as cms

ctppsDiamondTimingValidation = cms.EDAnalyzer('CTPPSDiamondTimingValidation',
    diamondRecHitsTag = cms.InputTag('ctppsDiamondRecHits'),
    diamondLocalTracksTag = cms.InputTag('ctppsDiamondLocalTracks'),
    verticesTag = cms.InputTag('offlinePrimaryVertices'),
    beamSpotTag = cms.InputTag('offlineBeamSpot'),
)
