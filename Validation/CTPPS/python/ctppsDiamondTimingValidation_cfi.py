import FWCore.ParameterSet.Config as cms

ctppsDiamondTimingValidation = cms.EDAnalyzer('CTPPSDiamondTimingValidation',
    diamondRecHitsTag = cms.InputTag('ctppsDiamondRecHits'),
    diamondLocalTracksTag = cms.InputTag('ctppsDiamondLocalTracks'),
    vertexTag = cms.InputTga('offlinePrimaryVertices'),
)
