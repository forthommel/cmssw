import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from SimPPS.DirectSimProducer.ppsDirectProtonSimulation_cff import *

directSimPPSTask = cms.Task(
    beamDivergenceVtxGenerator,
    ppsDirectProtonSimulation
)

directSimPPS = cms.Sequence(directSimPPSTask)
