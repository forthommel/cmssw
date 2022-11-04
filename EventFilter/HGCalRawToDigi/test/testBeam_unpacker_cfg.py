import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.load('EventFilter.HGCalRawToDigi.hgcalEmulatedFEDRawData_cfi')
process.load('EventFilter.HGCalRawToDigi.hgcalDigis_cfi')

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(threshold = cms.untracked.string('DEBUG'))
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(20))

# raw data source
#process.source = cms.Source("NewEventStreamFileReader",
#process.source = cms.Source("FedRawDataInputSource",
#    fileNames = cms.untracked.vstring(
#        'file:EventFilter/HGCalRawToDigi/data/ECOND_little_endian.bin',
#    )
#)
process.source = cms.Source("EmptySource")

process.hgcalEmulatedFEDRawData.inputs = cms.vstring(
    'file:/eos/cms/store/group/dpg_hgcal/tb_hgcal/2022/sps_oct2022/electron_beam_100_160fC/beam_run/run_20221009_222828/beam_run0.root',
)
process.hgcalDigis.rawDataTag = cms.InputTag('hgcalEmulatedFEDRawData')

process.p = cms.Path(
    process.hgcalEmulatedFEDRawData
    * process.hgcalDigis
)

# output configuration
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("hgcal_digi.root"),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_hgcalEmulatedFEDRawData_*_*',
        'keep *_hgcalDigis_*_*',
    )
)

process.outpath = cms.EndPath(process.output)
