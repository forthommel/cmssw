import FWCore.ParameterSet.Config as cms

genericStripsPackage = cms.PSet(
    # TODO: needed all below ?? 
    scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
    resolution = cms.double(12.e-6), # RP resolution, m
    minXi = cms.double(0.03),
    maxXi = cms.double(0.17),
)

# list of detector packages to simulate
# z coordinates in TOTEM/LHC coordinate system, in m
detectorPackages = cms.VPSet(
    #----- sector 45
    genericStripsPackage.clone(
        potId = cms.uint32(0x76180000), # 003
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb2'), # TODO: this is to be updated
        zPosition = cms.double(-212.551),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(2023227392), # 023
        interpolatorName = cms.string('ip5_to_station_150_h_2_lhcb2'), # TODO: this is to be updated
        zPosition = cms.double(-219.550),
    ),

    #----- sector 56
    genericStripsPackage.clone(
        potId = cms.uint32(0x77180000), # 103
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb1'), # TODO: this is to be updated
        zPosition = cms.double(+212.551),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(2040004608), # 123
        interpolatorName = cms.string('ip5_to_station_150_h_2_lhcb1'), # TODO: this is to be updated
        zPosition = cms.double(+219.550),
    ),
)
