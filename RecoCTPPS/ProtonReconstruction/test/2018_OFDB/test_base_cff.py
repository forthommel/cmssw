import FWCore.ParameterSet.Config as cms

# conditions DB
from CondCore.CondDB.CondDB_cfi import *

CondDB.connect = 'frontier://FrontierProd/CMS_CONDITIONS'

PoolDBESSource = cms.ESSource("PoolDBESSource",
    CondDB,
    DumpStat = cms.untracked.bool(False),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('LHCInfoRcd'),
        #tag = cms.string("LHCInfoTest_prompt_v3")
        tag = cms.string("LHCInfoEndFill_prompt_v1")
    )),
)

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
from RecoCTPPS.ProtonReconstruction.year_2018_OFDB.ctppsProtonReconstructionOFDB_cfi import *

# reconstruction plotter
ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOFDB"),

    rpId_45_F = cms.uint32(23),
    rpId_45_N = cms.uint32(3),
    rpId_56_N = cms.uint32(103),
    rpId_56_F = cms.uint32(123),

    outputFile = cms.string("reco_plots.root")
)

# processing sequence
p = cms.Path(
    ctppsProtonReconstructionOFDB

    * ctppsTrackDistributionPlotter
    * ctppsProtonReconstructionPlotter
)
