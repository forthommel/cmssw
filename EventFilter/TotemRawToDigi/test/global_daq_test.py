import FWCore.ParameterSet.Config as cms

process = cms.Process("TotemIntegratedRawDataTest")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# raw data source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/j/jkaspar/public/run273062_ls0001-2_stream.root')
#)
process.source = cms.Source("NewEventStreamFileReader",
    fileNames = cms.untracked.vstring('/store/t0streamer/Minidaq/A/000/276/395/run276395_ls0001_streamA_StorageManager.dat'),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# raw-to-digi conversion
process.load('CondFormats.TotemReadoutObjects.TotemDAQMappingESSourceXML_cfi')
#process.TotemDAQMappingESSourceXML.mappingFileNames.append("CondFormats/TotemReadoutObjects/xml/ctpps_210_mapping.xml")
process.TotemDAQMappingESSourceXML.mappingFileNames.append("CondFormats/TotemReadoutObjects/xml/timing_diamond_215_mapping.xml")

process.load("EventFilter.TotemRawToDigi.totemTriggerRawToDigi_cfi")
process.totemTriggerRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

process.load('EventFilter.TotemRawToDigi.totemRPRawToDigi_cfi')
process.totemRPRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

process.load('EventFilter.TotemRawToDigi.totemDiamondRPRawToDigi_cfi')
process.totemDiamondRPRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

# execution configuration
process.p = cms.Path(
    process.totemTriggerRawToDigi *
#    process.totemRPRawToDigi *
    process.totemDiamondRPRawToDigi
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root')
)
process.e = cms.EndPath(process.out)
