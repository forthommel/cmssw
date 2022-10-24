import FWCore.ParameterSet.Config as cms

process = cms.Process("PPS")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('Configuration.StandardSequences.Services_cff')
process.load('Geometry.VeryForwardGeometry.geometryRPFromDB_cfi')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_prompt')

process.GlobalTag.toGet.append(
    cms.PSet(
        connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
        record = cms.string('PPSTimingCalibrationRcd'),
        tag = cms.string('CTPPPSTimingCalibration_HPTDC_byPCL_v1_prompt'),
        label = cms.untracked.string('pcl'),
    )
)
#process.GlobalTag.DumpStat = cms.untracked.bool(True)
#process.load('Geometry.VeryForwardGeometry.geometryRPFromDD_2021_cfi')

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(359188),
    lastValue = cms.uint64(359579),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('testPPSTimingCalibrationAnalyser.root')
)

process.load('Geometry.VeryForwardGeometry.geometryRPFromDD_2021_cfi')
process.load('CalibPPS.TimingCalibration.ppsTimingCalibrationAnalyser_cfi')
process.ppsTimingCalibrationAnalyser.label = "pcl"

process.p = cms.Path(process.ppsTimingCalibrationAnalyser)
