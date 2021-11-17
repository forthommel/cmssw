import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2016
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2016_postTS2 as selected_optics

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2016_postTS2.xml")

profile_2016_postTS2 = profile_base_2016.clone(
    L_int = cms.double(5.007365807),
    ctppsLHCInfo = profile_base_2016.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2016_postTS2/h2_betaStar_vs_xangle")
    ),
    ctppsOpticalFunctions = profile_base_2016.ctppsOpticalFunctions.clone(
        opticalFunctions = selected_optics.opticalFunctions,
        scoringPlanes = selected_optics.scoringPlanes,
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2016.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    # direct simu data
    ctppsDirectSimuData = profile_base_2016.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("6.10374E-05+(([xi]<0.113491)*0.00795942+([xi]>=0.113491)*0.01935)*([xi]-0.113491)"),
        empiricalAperture56 = cms.string("([xi]-0.110)/130.0")
    )
)
