import FWCore.ParameterSet.Config as cms

ppxzGeneratorValidation = cms.EDAnalyzer("PPXZGeneratorValidation",
  verbosity = cms.untracked.uint32(0),

  hepMCTag = cms.InputTag("generator", "unsmeared"),
  recoTracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),

  outputFile = cms.string("ppxzGeneratorValidation.root")
)
