import FWCore.ParameterSet.Config as cms

# list of detector packages to simulate
detectorPackages_2016PreTS2 = cms.VPSet(
    cms.PSet(
        potId = cms.uint32(2),
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb2'),
        scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
        resolution = cms.double(12.e-6), # RP resolution, m
        minXi = cms.double(0.03),
        maxXi = cms.double(0.17),
    ),
    cms.PSet(
        potId = cms.uint32(3),
        interpolatorName = cms.string('ip5_to_station_150_h_2_lhcb2'),
        scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
        resolution = cms.double(12.e-6), # RP resolution, m
        minXi = cms.double(0.03),
        maxXi = cms.double(0.17),
    ),
    cms.PSet(
        potId = cms.uint32(102),
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb1'),
        scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
        resolution = cms.double(12.e-6), # RP resolution, m
        minXi = cms.double(0.03),
        maxXi = cms.double(0.17),
    ),
    cms.PSet(
        potId = cms.uint32(103),
        interpolatorName = cms.string('ip5_to_station_150_h_2_lhcb1'),
        scatteringAngle = cms.double(25.e-6), # physics scattering angle, rad
        resolution = cms.double(12.e-6), # RP resolution, m
        minXi = cms.double(0.03),
        maxXi = cms.double(0.17),
    ),
)
