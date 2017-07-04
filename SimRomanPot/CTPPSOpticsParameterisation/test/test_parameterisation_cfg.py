import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process('SIM', eras.Run2_25ns)

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("Configuration.StandardSequences.Services_cff")
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000),
)
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

process.source = cms.Source('EmptySource')

#process.options = cms.untracked.PSet()

process.VtxSmeared.src = cms.InputTag('lhcBeamProducer', 'unsmeared')
process.genParticles.src = cms.InputTag('lhcBeamProducer', 'unsmeared')
process.g4SimHits.Generator.HepMCProductLabel = cms.InputTag('lhcBeamProducer', 'unsmeared')

process.load('SimRomanPot.CTPPSOpticsParameterisation.lhcBeamProducer_cfi')
process.load('SimRomanPot.CTPPSOpticsParameterisation.ctppsOpticsParameterisation_cfi')

process.out = cms.OutputModule('PoolOutputModule',
    fileName = cms.untracked.string('ctppsSim.root')
)

process.RandomNumberGeneratorService.lhcBeamProducer = cms.PSet(
    initialSeed = cms.untracked.uint32(1),
    #engineName = cms.untracked.string('TRandom3'),
)
# for detectors resolution smearing
process.RandomNumberGeneratorService.ctppsOpticsParameterisation = cms.PSet( initialSeed = cms.untracked.uint32(1), )

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    #outputCommands = process.RAWSIMEventContent.outputCommands,
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('file:output_step1_GEN-SIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
)

process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_RealisticBS_25ns_13TeV2016_v1_mc', '')
#process.ProductionFilterSequence = cms.Sequence(process.generator)


# Path and EndPath definitions
process.generation_step = cms.Path(
    process.lhcBeamProducer
    * process.pgen
)
process.simulation_step = cms.Path(
#    process.psim
    process.ctppsOpticsParameterisation
)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(
    process.generation_step,
    process.genfiltersummary_step,
    process.simulation_step,
    process.endjob_step,
    process.RAWSIMoutput_step
)
