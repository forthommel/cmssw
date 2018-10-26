import FWCore.ParameterSet.Config as cms

# minimum of logs
MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring()
)

# track distribution plotter
ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
    tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
    outputFile = cms.string("track_plots.root")
)

# proton reconstruction
from RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi import *

# reconstruction plotter
ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOF"),

    rpId_45_F = cms.uint32(23),
    rpId_45_N = cms.uint32(3),
    rpId_56_N = cms.uint32(103),
    rpId_56_F = cms.uint32(123),

    outputFile = cms.string("reco_plots.root")
)

# processing sequence
p = cms.Path(
    ctppsProtonReconstructionOF

    * ctppsTrackDistributionPlotter
    * ctppsProtonReconstructionPlotter
)
