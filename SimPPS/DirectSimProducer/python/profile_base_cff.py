import FWCore.ParameterSet.Config as cms
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2017, optics_2018, optics_2021, optics_2022

profile_base = cms.PSet(
    L_int = cms.double(1.),
    # LHCInfo
    ctppsLHCInfo = cms.PSet(
        xangle = cms.double(-1.),
        betaStar = cms.double(-1.),
        beamEnergy = cms.double(0.),
        xangleBetaStarHistogramFile = cms.string("CalibPPS/ESProducers/data/xangle_beta_distributions/version1.root"),
        xangleBetaStarHistogramObject = cms.string("")
    ),
    # optics
    ctppsOpticalFunctions = cms.PSet(
      	opticalFunctions = cms.VPSet(),
  	    scoringPlanes = cms.VPSet()
    ),
    # alignment
    ctppsRPAlignmentCorrectionsDataXML = cms.PSet(
        MeasuredFiles = cms.vstring(),
        RealFiles = cms.vstring(),
        MisalignedFiles = cms.vstring()
    ),
    # direct simu data
    ctppsDirectSimuData = cms.PSet(
        empiricalAperture45 = cms.string(""),
        empiricalAperture56 = cms.string(""),
        timeResolutionDiamonds45 = cms.string("999"),
        timeResolutionDiamonds56 = cms.string("999"),
        efficienciesPerRP = cms.VPSet(),
        efficienciesPerPlane = cms.VPSet()
    )
)

# base profile settings for 2016
profile_base_2016 = profile_base.clone(
    ctppsLHCInfo = profile_base.ctppsLHCInfo.clone(
        beamEnergy = 6500
    )
)

# base profile settings for 2017
profile_base_2017 = profile_base_2016.clone(
    ctppsOpticalFunctions = profile_base.ctppsOpticalFunctions.clone(
        opticalFunctions = optics_2017.opticalFunctions,
        scoringPlanes = optics_2017.scoringPlanes,
    )
)

# base profile settings for 2018
profile_base_2018 = profile_base_2016.clone(
    ctppsOpticalFunctions = profile_base.ctppsOpticalFunctions.clone(
        opticalFunctions = optics_2018.opticalFunctions,
        scoringPlanes = optics_2018.scoringPlanes,
    ),
    ctppsDirectSimuData = profile_base.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("-(8.44219E-07*[xangle]-0.000100957)+(([xi]<(0.000247185*[xangle]+0.101599))*-(1.40289E-05*[xangle]-0.00727237)+([xi]> = (0.000247185*[xangle]+0.101599))*-(0.000107811*[xangle]-0.0261867))*([xi]-(0.000247185*[xangle]+0.101599))"),
        empiricalAperture56 = cms.string("-(-4.74758E-07*[xangle]+3.0881E-05)+(([xi]<(0.000727859*[xangle]+0.0722653))*-(2.43968E-05*[xangle]-0.0085461)+([xi]> = (0.000727859*[xangle]+0.0722653))*-(7.19216E-05*[xangle]-0.0148267))*([xi]-(0.000727859*[xangle]+0.0722653))")
    )
)

# base profile settings for 2021
profile_base_2021 = profile_base.clone(
    ctppsLHCInfo = profile_base.ctppsLHCInfo.clone(
        beamEnergy = cms.double(7000.)
    ),
    ctppsOpticalFunctions = profile_base.ctppsOpticalFunctions.clone(
        opticalFunctions = optics_2021.opticalFunctions,
        scoringPlanes = optics_2021.scoringPlanes,
    ),
    ctppsDirectSimuData = profile_base.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("1E3*([xi] - 0.20)"),
        empiricalAperture56 = cms.string("1E3*([xi] - 0.20)")
    )
)

# base profile settings for 2022
profile_base_2022 = profile_base_2021.clone(
    ctppsOpticalFunctions = profile_base.ctppsOpticalFunctions.clone(
        opticalFunctions = optics_2022.opticalFunctions,
        scoringPlanes = optics_2022.scoringPlanes,
    ),
)

def matchDirectSimOutputs(process):

    process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsPixelLocalTracks.tag = cms.InputTag('ppsDirectProtonSimulation')
    process.ctppsDiamondLocalTracks.recHitsTag = cms.InputTag('ppsDirectProtonSimulation')
