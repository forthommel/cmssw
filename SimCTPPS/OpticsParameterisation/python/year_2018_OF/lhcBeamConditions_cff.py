import FWCore.ParameterSet.Config as cms

lhcBeamConditions = cms.PSet(
    sqrtS = cms.double(13.e3), # in GeV
    vertexSize = cms.double(10.e-6), # in m
    beamDivergence = cms.double(20.e-6), # in rad

    # vertex vertical offset in both sectors
    yOffsetSector45 = cms.double(0e-6), # in m
    yOffsetSector56 = cms.double(0.e-6), # in m

    # crossing angle
    halfCrossingAngleSector45 = cms.double(140e-6), # in rad
    halfCrossingAngleSector56 = cms.double(140e-6), # in rad
)
