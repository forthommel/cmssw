import FWCore.ParameterSet.Config as cms

smearingValidator = cms.EDAnalyzer("SmearingValidator",
  tagBeforeSmearing = cms.InputTag(""),
  tagAfterSmearing = cms.InputTag(""),

  outputFile = cms.string("smearingValidator.root")
)
