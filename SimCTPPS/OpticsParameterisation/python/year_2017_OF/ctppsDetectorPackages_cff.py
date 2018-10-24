import FWCore.ParameterSet.Config as cms

genericStripsPackage = cms.PSet(
    # TODO: needed all below ?? 
    scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
    resolution = cms.double(12.e-6), # RP resolution, m
    minXi = cms.double(0.03),
    maxXi = cms.double(0.17),
    maxX = cms.double(100.) # mm
)

# list of detector packages to simulate
# z coordinates in TOTEM/LHC coordinate system, in m
detectorPackages = cms.VPSet(
    #----- sector 45
    genericStripsPackage.clone(
        potId = cms.uint32(0x76180000), # 003
        interpolatorName = cms.string('45-210-fr'),
        zPosition = cms.double(-212.551),
        maxX = cms.double(15.5),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(2023227392), # 023
        interpolatorName = cms.string('45-220-fr'),
        zPosition = cms.double(-219.550),
        maxX = cms.double(15.5),
    ),

    #----- sector 56
    genericStripsPackage.clone(
        potId = cms.uint32(0x77180000), # 103
        interpolatorName = cms.string('56-210-fr'),
        zPosition = cms.double(+212.551),
        maxX = cms.double(14.),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(2040004608), # 123
        interpolatorName = cms.string('56-220-fr'),
        zPosition = cms.double(+219.550),
        maxX = cms.double(14.),
    ),
)
