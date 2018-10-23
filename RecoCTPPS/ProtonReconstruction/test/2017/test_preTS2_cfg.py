import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSTestProtonReconstruction", eras.ctpps_2016)

# config
run = "299395"
input_file = "/store/data/Run2017C/DoubleEG/AOD/17Nov2017-v1/20000/E28C8EAC-C1D2-E711-9EAF-24BE05C63681.root"


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
  fileNames = cms.untracked.vstring(input_file),
  lumisToProcess = cms.untracked.VLuminosityBlockRange(run + ":1-" + run + ":max")
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
    tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
    outputFile = cms.string("run_" + run + "_track_plots.root")
)

from SimCTPPS.OpticsParameterisation.year_2017.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017.lhcBeamConditions_cff import lhcBeamConditions

process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.beamConditions = lhcBeamConditions
process.ctppsProtonReconstruction.detectorPackages = detectorPackages

# TODO: update
process.ctppsProtonReconstruction.opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root')
process.ctppsProtonReconstruction.opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root')

process.ctppsProtonReconstruction.applyExperimentalAlignment = True
process.ctppsProtonReconstruction.alignmentFile = "RecoCTPPS/ProtonReconstruction/data/preliminary_alignment_2017.out"

process.ctppsProtonReconstruction.verbosity = 0

process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstruction"),
    outputFile = cms.string("run_" + run + "_reco_plots.root")
)

process.p = cms.Path(
    process.ctppsProtonReconstruction

    * process.ctppsTrackDistributionPlotter
    * process.ctppsProtonReconstructionPlotter
)
