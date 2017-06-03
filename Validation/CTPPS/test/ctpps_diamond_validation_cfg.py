import FWCore.ParameterSet.Config as cms

process = cms.Process("CTPPSDiamondTimingValid")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/023FE845-4648-E711-ABBD-02163E01451C.root'
    )
)

process.load('Validation.CTPPS.ctppsDiamondTimingValidation_cfi')

process.p = cms.Path(
    process.ctppsDiamondTimingValidation
)
