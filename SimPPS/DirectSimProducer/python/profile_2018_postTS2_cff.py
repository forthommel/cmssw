import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2018

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2018_postTS2.xml")

profile_2018_postTS2 = profile_base_2018.clone(
    L_int = cms.double(10.415769561),
    ctppsLHCInfo = profile_base_2018.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2018_postTS2/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2018.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2018.ctppsDirectSimuData.clone(
        timeResolutionDiamonds45 = cms.string("2*((x<16)*(-0.381504+0.255095*x-0.0415622*x^2+0.00275877*x^3-6.47115E-05*x^4)+(x>=16)*0.118)"),
        timeResolutionDiamonds56 = cms.string("2*((x<16)*(-0.279298+0.219838*x-0.0384257*x^2+0.00268906*x^3-6.60572E-05*x^4)+(x>=16)*0.099)"),
    )
)
