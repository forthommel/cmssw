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

# data source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("/store/data/Run2017D/DoubleEG/MINIAOD/17Nov2017-v1/70000/90E543DB-42D5-E711-9F5B-0242AC1C0501.root")

  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/preTS2_alignment_data/version4/fill5839_xangle140_DoubleEG.root")
  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/preTS2_alignment_data/version4/fill6091_xangle140_DoubleEG.root")
  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/preTS2_alignment_data/version4/fill6192_xangle140_DoubleEG.root")

  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/postTS2_alignment_data/version5/fill6371_xangle130_DoubleEG.root")
  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/postTS2_alignment_data/version5/fill6371_xangle140_DoubleEG.root")
  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/postTS2_alignment_data/version5/fill6371_xangle150_DoubleEG.root")
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100000)
)

# track distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("track_plots_xangle.root")
)

# proton reconstruction
process.load("RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi")
#process.ctppsProtonReconstructionOFDB.fitVtxY = False
#process.ctppsProtonReconstructionOFDB.verbosity = 0

# reconstruction plotter
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOFDB"),

    rpId_45_F = cms.uint32(23),
    rpId_45_N = cms.uint32(3),
    rpId_56_N = cms.uint32(103),
    rpId_56_F = cms.uint32(123),

    outputFile = cms.string("reco_plots_xangle.root")
)

# processing sequence
process.p = cms.Path(
  process.ctppsProtonReconstructionOFDB
  * process.ctppsTrackDistributionPlotter
  * process.ctppsProtonReconstructionPlotter
)

# select only data corresponding to certain crossing-angle
from RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi import SelectCrossingAngle
SelectCrossingAngle(130, process.source)
