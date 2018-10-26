import FWCore.ParameterSet.Config as cms

from SimCTPPS.OpticsParameterisation.year_2017_OF.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017_OF.lhcBeamConditions_cff import lhcBeamConditions

ctppsProtonReconstructionOF = cms.EDProducer('CTPPSProtonReconstructionOF',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    beamConditions = lhcBeamConditions,
    detectorPackages = detectorPackages,

    opticsFileBeam1 = cms.FileInPath(""),
    opticsFileBeam2 = cms.FileInPath(""),

    applyExperimentalAlignment = cms.bool(True),
    alignmentFiles = cms.vstring(
      "RecoCTPPS/ProtonReconstruction/data/alignment/2017_preTS2/collect_alignments_2018_10_26.1.out",
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

#def UseCrossingAngle100():
#  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_100.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#
#def UseCrossingAngle110():
#  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_110.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#
#def UseCrossingAngle120():
#  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_120.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_120.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#
#def UseCrossingAngle130():
#  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_130.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_130.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)

def UseCrossingAngle140(source):
  ctppsProtonReconstructionOF.opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root')
  ctppsProtonReconstructionOF.opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root')

  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)
  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_140.json").getCMSSWString().split(',')
  source.lumisToProcess.extend(myLumis)

#def UseCrossingAngle150():
#  source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_preTS2/xangle_150.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis)
#  myLumis = LumiList.LumiList(filename = "../../data/json/2017_postTS2/xangle_150.json").getCMSSWString().split(',')
#  source.lumisToProcess.extend(myLumis
