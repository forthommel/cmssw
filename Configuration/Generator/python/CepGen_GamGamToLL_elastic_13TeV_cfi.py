import FWCore.ParameterSet.Config as cms

from GeneratorInterface.CepGenInterface.cepgenDefaultParameters_cff import *

generator = cms.EDFilter("CepGenGeneratorFilter",
    process = cms.PSet(
        name = cms.string('lpair'),
        processParameters = cms.PSet(
            mode = cms.uint32(1),  # 1 = elastic, 2-3 = SD, 4 = DD
            pair = cms.uint32(13)
        ),
        inKinematics = cms.PSet(
            cmEnergy = cms.double(13.e3),
        ),
        outKinematics = cms.PSet(
            pt = cms.vdouble(25.,),
            eta = cms.vdouble(-2.5, 2.5),
        )
    ),
    outputModules = cepgenOutputModules,
    maxEventsToPrint = cms.untracked.int32(0),
)
