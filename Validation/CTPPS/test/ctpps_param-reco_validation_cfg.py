import FWCore.ParameterSet.Config as cms

process = cms.Process('VALID')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.Services_cff")
process.load('Validation.CTPPS.ctppsReconstruction_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000),
)
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
'file:ctppsSim.root'
    )
)

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True),
)

process.p = cms.Path(
    process.ctppsReconstruction
)
