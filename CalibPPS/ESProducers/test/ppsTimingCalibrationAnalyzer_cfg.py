import FWCore.ParameterSet.Config as cms

process = cms.Process('test')

# minimum logging
process.MessageLogger = cms.Service('MessageLogger',
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        #threshold = cms.untracked.string('INFO')
        threshold = cms.untracked.string('ERROR')
    )
)

process.source = cms.Source('EmptyIOVSource',
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

# load calibrations from database
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
# load calibrations from SQLite file
#process.load('CondCore.CondDB.CondDB_cfi')
#process.CondDB.connect = 'sqlite_file:ppsTiming_calibration.sqlite' # SQLite input
#process.PoolDBESSource = cms.ESSource('PoolDBESSource',
#    process.CondDB,
#    DumpStats = cms.untracked.bool(True),
#    toGet = cms.VPSet(
#        cms.PSet(
#            record = cms.string('PPSTimingCalibrationRcd'),
#            tag = cms.string('PPSDiamondTimingCalibration')
#        )
#    )
#)

process.ppsTimingCalibrationAnalyzer = cms.EDAnalyzer('PPSTimingCalibrationAnalyzer',
    #timingCalibrationTag = cms.string('GlobalTag:TotemTimingCalibration')
    timingCalibrationTag = cms.string('GlobalTag')
)

process.path = cms.Path(
    process.ppsTimingCalibrationAnalyzer
)

