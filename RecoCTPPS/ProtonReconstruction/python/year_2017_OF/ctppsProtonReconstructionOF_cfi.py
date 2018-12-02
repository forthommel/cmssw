import FWCore.ParameterSet.Config as cms

from SimCTPPS.OpticsParameterisation.year_2017_OF.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017_OF.lhcBeamConditions_cff import lhcBeamConditions

ctppsProtonReconstructionOF = cms.EDProducer('CTPPSProtonReconstructionOF',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    beamConditions = lhcBeamConditions,
    detectorPackages = detectorPackages,

    xangle = cms.double(0), # in urad

    xangle1 = cms.double(140),
    opticsFile1 = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root"),

    xangle2 = cms.double(120),
    opticsFile2 = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_120urad.root"),

    applyExperimentalAlignment = cms.bool(True),
    alignmentFiles = cms.vstring(
      "RecoCTPPS/ProtonReconstruction/data/alignment/2017_preTS2/collect_alignments_2018_10_26.4.out",
      "RecoCTPPS/ProtonReconstruction/data/alignment/2017_postTS2/collect_alignments_2018_10_25.5.out"
    ),

    applyTestAlignment = cms.bool(False),
    de_x_N = cms.double(0.),
    de_x_F = cms.double(0.),
    de_y_N = cms.double(0.),
    de_y_F = cms.double(0.)
)

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
import os

cmsswBase = os.environ["CMSSW_BASE"]

def UseCrossingAngle100(source):
  ctppsProtonReconstructionOF.xangle = 100

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_100.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle110(source):
  ctppsProtonReconstructionOF.xangle = 110

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_110.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle120(source):
  ctppsProtonReconstructionOF.xangle = 120

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_120.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_120.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle130(source):
  ctppsProtonReconstructionOF.xangle = 130

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_130.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_130.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle140(source):
  ctppsProtonReconstructionOF.xangle = 140

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = cmsswBase + "/src/RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle150(source):
  ctppsProtonReconstructionOF.xangle = 150

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_150.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_150.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle(xangle, source):
  if (xangle == 100):
    UseCrossingAngle100(source)
  elif (xangle == 110):
    UseCrossingAngle110(source)
  elif (xangle == 120):
    UseCrossingAngle120(source)
  elif (xangle == 130):
    UseCrossingAngle130(source)
  elif (xangle == 140):
    UseCrossingAngle140(source)
  elif (xangle == 150):
    UseCrossingAngle150(source)
  else:
    print("ERROR: crossing angle " + str(xangle) + " is not supported.")
