import FWCore.ParameterSet.Config as cms

from RecoCTPPS.TotemRPLocal.totemRPLocalReconstruction_cff import *
recoCTPPS  = cms.Sequence(totemRPLocalReconstruction)

from RecoCTPPS.ProtonProducer.protonProducer_cfi import protons
protonProducer = cms.Sequence(protons)
