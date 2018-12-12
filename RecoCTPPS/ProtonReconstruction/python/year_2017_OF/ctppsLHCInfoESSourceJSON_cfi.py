import FWCore.ParameterSet.Config as cms

import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

import os

cmsswBase = os.environ["CMSSW_BASE"]

def GetRange(file):
  lumiList = LumiList.LumiList(filename = cmsswBase + "/src/" + file).getCMSSWString().split(',')
  output = cms.VLuminosityBlockRange()
  output.extend(lumiList)
  return output

base = cms.PSet(
  beamEnergy = cms.double(6500),  # GeV
  xangle = cms.double(100),  # murad
  ranges = cms.VLuminosityBlockRange()
)

ctppsLHCInfoESSourceJSON = cms.ESSource("CTPPSLHCInfoESSourceJSON",
  configuration = cms.VPSet(
    base.clone( xangle = 120, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_120.json") ),
    base.clone( xangle = 130, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_130.json") ),
    base.clone( xangle = 140, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_140.json") ),
    base.clone( xangle = 150, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_preTS2/xangle_150.json") ),

    base.clone( xangle = 100, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_100.json") ),
    base.clone( xangle = 110, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_110.json") ),
    base.clone( xangle = 120, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_120.json") ),
    base.clone( xangle = 130, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_130.json") ),
    base.clone( xangle = 140, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_140.json") ),
    base.clone( xangle = 150, ranges = GetRange("RecoCTPPS/ProtonReconstruction/data/json/2017_postTS2/xangle_150.json") )
  )
)
