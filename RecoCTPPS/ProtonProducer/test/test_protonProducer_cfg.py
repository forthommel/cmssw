import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process('protonReco')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.GlobalTag = GlobalTag(process.GlobalTag,'80X_dataRun2_Prompt_v9')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/group/phys_diffraction/lforthom/DoubleEG/miniAOD_pps_run2016G_doubleeg_v2/161006_001937/0000/miniAOD_PAT_22.root',
    )
)

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('output.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_protons_*_*',
    )
)

process.load('RecoCTPPS.ProtonProducer.protonProducer_cfi')
process.p = cms.Path(process.protons)

process.e = cms.EndPath(process.out)
