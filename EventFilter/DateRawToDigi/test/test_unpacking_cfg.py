import FWCore.ParameterSet.Config as cms

process = cms.Process('DATE')

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

# raw data source
process.source = cms.Source('EmptySource')
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
)

# raw-to-digi conversion
process.load("EventFilter.DateRawToDigi.dateDigis_cff")
process.dateDigis.inputFile = cms.string('file:///eos/home-f/fgarciaf/TOTEM_BeamTest_At_SPS_H8/September2018/DATE/Run000.raw')

# execution configuration
process.p = cms.Path(
  process.dateDigis
)

# output configuration
process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string("file:./reco_digi.root"),
  outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_*RawToDigi_*_*',
  )
)

process.outpath = cms.EndPath(process.output)
