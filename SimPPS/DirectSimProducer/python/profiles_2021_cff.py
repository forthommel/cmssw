import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base as _base
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2021 as _optics

# base profile settings for 2021
_base_2021 = _base.clone(
    ctppsOpticalFunctions = _base.ctppsOpticalFunctions.clone(
        opticalFunctions = _optics.opticalFunctions,
        scoringPlanes = _optics.scoringPlanes,
    ),
    ctppsDirectSimuData = _base.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("1E3*([xi] - 0.20)"),
        empiricalAperture56 = cms.string("1E3*([xi] - 0.20)")
    )
)

profile_2021_default = _base_2021.clone(
    L_int = cms.double(1.),
    ctppsLHCInfo = _base_2021.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2021/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = _base_2021.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = cms.vstring("Validation/CTPPS/alignment/2021.xml"),
        RealFiles = cms.vstring("Validation/CTPPS/alignment/2021.xml")
    ),
    ctppsDirectSimuData = _base_2021.ctppsDirectSimuData.clone(
        timeResolutionDiamonds45 = cms.string("0.200"),
        timeResolutionDiamonds56 = cms.string("0.200")
    )
)
