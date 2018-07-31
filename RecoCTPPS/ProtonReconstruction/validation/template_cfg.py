import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSTestProtonReconstruction", eras.ctpps_2016)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# raw data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring()
  #lumisToProcess = cms.untracked.VLuminosityBlockRange("$run:1-$run:max")
)
$input

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.applyExperimentalAlignment = True
process.ctppsProtonReconstruction.alignmentFile = "RecoCTPPS/ProtonReconstruction/data/collect_alignments_$alignment.out"

process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstruction"),
    outputFile = cms.string("$output"),
    maxNonEmptyEvents = cms.untracked.int32(200000)
)

process.p = cms.Path(
    process.ctppsProtonReconstruction
    * process.ctppsProtonReconstructionPlotter
)
