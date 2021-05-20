import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
totemT2DQMSource = DQMEDAnalyzer('TotemT2DQMSource',
    statusLabel = cms.InputTag("totemT2Digis", "TotemT2"),
    digiLabel = cms.InputTag("totemT2Digis", "TotemT2"),
    fedInfoLabel = cms.InputTag("totemT2Digis", "TotemT2"),
    rechitsLabel = cms.InputTag("totemT2RecHits"),
)
