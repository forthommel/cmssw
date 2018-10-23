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
  fileNames = cms.untracked.vstring(
    # run 275371
    "/store/data/Run2016B/DoubleEG/MINIAOD/18Apr2017_ver2-v1/00000/00220DCF-073E-E711-AB1A-0025905C2CBA.root"

    # run 279766
    #"/store/data/Run2016G/DoubleEG/MINIAOD/07Aug17-v1/50000/FE980B48-B09E-E711-9742-00259048C284.root"
  ),
  lumisToProcess = cms.untracked.VLuminosityBlockRange("275371:1-275371:999999")
  #lumisToProcess = cms.untracked.VLuminosityBlockRange("279766:1-279766:999999")
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.applyExperimentalAlignment = True
process.ctppsProtonReconstruction.alignmentFiles = cms.vstring("RecoCTPPS/ProtonReconstruction/data/alignment/2016_preTS2/collect_alignments_2018_07_30.5.out")

process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstruction"),
    outputFile = cms.string("test_data_plots_275371.root")
    #outputFile = cms.string("test_data_plots_279766.root")
)

process.p = cms.Path(
    process.ctppsProtonReconstruction
    * process.ctppsProtonReconstructionPlotter
)
