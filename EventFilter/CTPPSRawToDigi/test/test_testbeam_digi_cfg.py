import FWCore.ParameterSet.Config as cms

process = cms.Process('PPS')

# a bit about the run (can do some extra checks in your EDProducer to ensure the
# maximum number of events generated does not exceed the ones stored in the tree)
process.source = cms.Source('EmptySource')
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# define the output
process.output = cms.OutputModule('PoolOutputModule',
    fileName = cms.untracked.string('file:output.root'),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_totemTimingTestBeamDigis_*_*',
    ),
)

# either define your mapping (+other configuration parameters for the EDProducer) here
process.totemTimingTestBeamDigis = cms.EDProducer('TotemTimingTestBeamRawToDigi',
    idsMapping = cms.VPSet(
        cms.PSet(detId = cms.uint32(2088763392), treeChId = cms.uint32(0)),
        cms.PSet(detId = cms.uint32(2088767488), treeChId = cms.uint32(1)),
        cms.PSet(detId = cms.uint32(2088894464), treeChId = cms.uint32(2))
    ),
)
# or load it directly from the python configuration automatically generated
# by the fillDescriptions static member of your EDProducer
# (see your $CMSSW_BASE/cfipython directory structure)
#process.load('EventFilter.CTPPSRawToDigi.totemTimingTestBeamDigis_cfi')

# job definition
process.p = cms.Path(
    process.totemTimingTestBeamDigis
)
# output definition
process.outpath = cms.EndPath(process.output)
