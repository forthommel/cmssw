import FWCore.ParameterSet.Config as cms

ctppsProtonReconstructionValidator = cms.EDAnalyzer("CTPPSProtonReconstructionValidator",
  tagHepMCBeforeSmearing = cms.InputTag(""),
  tagHepMCAfterSmearing = cms.InputTag(""),
  tagRecoProtons = cms.InputTag(""),

  outputFile = cms.string("ctppsProtonReconstructionValidator.root")
)
