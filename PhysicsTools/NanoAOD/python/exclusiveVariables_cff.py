import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.NanoAOD.simpleSecondaryVertexFlatTableProducer_cfi import simpleSecondaryVertexFlatTableProducer


exclusiveVariablesTable = cms.EDProducer("ExclusiveVariablesTableProducer",
    vertices = cms.required.InputTag('offlineSlimmedPrimaryVertices'),
    electrons = cms.required.InputTag('slimmedElectrons'),
    muons = cms.required.InputTag('slimmedMuons'),
    pfCandidates = cms.InputTag("packedPFCandidates"),
    goodVerticesCuts = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    max3DDistance = cms.double(0.01),
)

exclusiveVariablesTablesTask = cms.Task(exclusiveVariablesTable)
