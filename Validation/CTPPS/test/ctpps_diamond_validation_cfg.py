import FWCore.ParameterSet.Config as cms

process = cms.Process("CTPPSDiamondTimingValid")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/023FE845-4648-E711-ABBD-02163E01451C.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/0278D47B-4A48-E711-A918-02163E014508.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/04DDBFB7-3E48-E711-8D56-02163E01387A.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/04FAB72B-3D48-E711-97EC-02163E01197B.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/04FEFA5E-4A48-E711-95CB-02163E013690.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/0679A149-4648-E711-936F-02163E019D9E.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/0882FE66-4348-E711-8744-02163E019D34.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/0A6E77BD-3548-E711-B381-02163E019DB9.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/10B6CC92-3F48-E711-BF85-02163E014531.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/10EAAE67-4A48-E711-A7F2-02163E01186D.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/10F0F45E-3948-E711-B640-02163E0136A4.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/1465B5B0-3F48-E711-85F0-02163E013889.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/16A3A307-3D48-E711-B83C-02163E01423B.root',
        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/977/00000/16A5875A-4A48-E711-826B-02163E014732.root',
    )
)

process.load('Validation.CTPPS.ctppsDiamondTimingValidation_cfi')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("output.root")
)

process.p = cms.Path(
    process.ctppsDiamondTimingValidation
)
