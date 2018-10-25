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

# proton reconstruction
from SimCTPPS.OpticsParameterisation.year_2017_OF.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017_OF.lhcBeamConditions_cff import lhcBeamConditions

process.ctppsProtonReconstructionOF = cms.EDProducer('CTPPSProtonReconstructionOF',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    beamConditions = lhcBeamConditions,
    detectorPackages = detectorPackages,

    opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root'),
    opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root'),

    applyExperimentalAlignment = cms.bool(True),
    alignmentFiles = cms.vstring("RecoCTPPS/ProtonReconstruction/data/alignment/2017_postTS2/collect_alignments_$alignment.out"),

    applyTestAlignment = cms.bool(False),
    de_x_N = cms.double(0.),
    de_x_F = cms.double(0.),
    de_y_N = cms.double(0.),
    de_y_F = cms.double(0.)
)

# reconstruction plotter
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOF"),

    rpId_45_F = cms.uint32(23),
    rpId_45_N = cms.uint32(3),
    rpId_56_N = cms.uint32(103),
    rpId_56_F = cms.uint32(123),

    outputFile = cms.string("$output"),
    maxNonEmptyEvents = cms.untracked.int32(100000)
)

process.p = cms.Path(
    process.ctppsProtonReconstructionOF
    * process.ctppsProtonReconstructionPlotter
)
