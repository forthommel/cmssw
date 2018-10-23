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

ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
    tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
    outputFile = cms.string("track_plots.root")
)

from SimCTPPS.OpticsParameterisation.year_2017.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017.lhcBeamConditions_cff import lhcBeamConditions

from RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi import *
ctppsProtonReconstruction.beamConditions = lhcBeamConditions
ctppsProtonReconstruction.detectorPackages = detectorPackages

# TODO: update
ctppsProtonReconstruction.opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root')
ctppsProtonReconstruction.opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root')

ctppsProtonReconstruction.applyExperimentalAlignment = True

ctppsProtonReconstruction.alignmentFiles = cms.vstring(
    "RecoCTPPS/ProtonReconstruction/data/alignment/2017_preTS2/preliminary_alignment.out",
    "RecoCTPPS/ProtonReconstruction/data/alignment/2017_postTS2/collect_alignments_2018_10_23.1.out"
)

ctppsProtonReconstruction.verbosity = 0

ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstruction"),
    outputFile = cms.string("reco_plots.root")
)

p = cms.Path(
    ctppsProtonReconstruction

    * ctppsTrackDistributionPlotter
    * ctppsProtonReconstructionPlotter
)

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

# from 100

def UseCrossingAngle100():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_100.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle110():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_110.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle120():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_120.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_120.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle130():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_130.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_130.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle140():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle150():
  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_150.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_150.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

