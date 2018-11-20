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

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/preTS2_alignment_data/version1/fill6031_xangle140_DoubleEG.root")
  #fileNames = cms.untracked.vstring("root://eostotem.cern.ch//eos/totem/data/ctpps/reconstruction/2017/postTS2_alignment_data/version5/fill6370_xangle140_DoubleEG.root")
)

# track distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
    tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
    outputFile = cms.string("data.root")
)

# processing sequence
process.p = cms.Path(
    process.ctppsTrackDistributionPlotter
)
