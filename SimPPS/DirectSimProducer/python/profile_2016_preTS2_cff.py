import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2016
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2016_preTS2 as selected_optics

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2016_preTS2.xml")

profile_2016_preTS2 = profile_base_2016.clone(
    L_int = cms.double(6.138092276 + 3.654039035),
    ctppsLHCInfo = profile_base_2016.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2016_preTS2/h2_betaStar_vs_xangle")
    ),
    ctppsOpticalFunctions = profile_base_2016.ctppsOpticalFunctions.clone(
        opticalFunctions = selected_optics.opticalFunctions,
        scoringPlanes = selected_optics.scoringPlanes,
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2016.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2016.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("3.76296E-05+(([xi]<0.117122)*0.00712775+([xi]>=0.117122)*0.0148651)*([xi]-0.117122)"),
        empiricalAperture56 = cms.string("1.85954E-05+(([xi]<0.14324)*0.00475349+([xi]>=0.14324)*0.00629514)*([xi]-0.14324)")
    )
)
