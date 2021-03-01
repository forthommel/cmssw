import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('SIMDQM', Run3)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.verbosity = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')

# load direct simulation (run 3)
process.load('Validation.CTPPS.simu_config.year_2021_cff')

# load DQM framework
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder = "CTPPS"
process.dqmEnv.eventInfoFolder = "EventInfo"
process.dqmSaver.path = ""
process.dqmSaver.tag = "CTPPS"

# diamond DQM module
process.load('DQM.CTPPS.ctppsDiamondDQMSource_cfi')
#process.ctppsDiamondDQMSource.excludeMultipleHits = cms.bool(True)
process.ctppsDiamondDQMSource.extractDigiInfo = cms.bool(False)
process.ctppsDiamondDQMSource.tagDiamondRecHits = cms.InputTag('ctppsDirectProtonSimulation')
#process.ctppsLocalTrackLiteProducer.includeDiamonds = cms.bool(True)
process.ctppsDiamondLocalReconstructionTask = process.ctppsDiamondLocalReconstructionTask.copyAndExclude([
    process.ctppsDiamondRecHits
])

process.path = cms.Path(
    process.generator *
    process.beamDivergenceVtxGenerator *
    process.ctppsDirectProtonSimulation *
    process.reco_local *
    #process.ctppsProtons *
    process.ctppsDiamondDQMSource
)

process.end_path = cms.EndPath(
    process.dqmEnv +
    process.dqmSaver
)

process.schedule = cms.Schedule(
    process.path,
    process.end_path
)
