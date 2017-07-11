import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process('PPS')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.Services_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
)
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source('PoolSource',
    #fileNames = cms.untracked.vstring('file:output_GEN-SIM.root'),
    fileNames = cms.untracked.vstring('file:output_step1_GEN-SIM.root'),
)

process.load('SimRomanPot.CTPPSOpticsParameterisation.ctppsOpticsParameterisation_cfi')
process.load('SimRomanPot.CTPPSOpticsParameterisation.ctppsOpticsReconstruction_cfi')

process.ctppsOpticsParameterisation.useBeamSpotPosition = cms.bool(False)

# for detectors resolution smearing
process.RandomNumberGeneratorService.ctppsOpticsParameterisation = cms.PSet( initialSeed = cms.untracked.uint32(1), )

process.load('Validation.CTPPS.ctppsParameterisationValidation_cfi')

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True),
)

process.out = cms.OutputModule('PoolOutputModule',
    fileName = cms.untracked.string('ctppsSim.root')
)

process.p = cms.Path(
    process.ctppsOpticsParameterisation
    * process.ctppsOpticsReconstruction
    * process.paramValidation
)

process.e = cms.EndPath(process.out)

