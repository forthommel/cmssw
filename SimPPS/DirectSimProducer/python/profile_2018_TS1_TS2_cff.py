import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2018

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2018_TS1_TS2.xml")

profile_2018_TS1_TS2 = profile_base_2018.clone(
    L_int = cms.double(26.812002394),
    ctppsLHCInfo = profile_base_2018.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2018_TS1_TS2/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2018.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2018.ctppsDirectSimuData.clone(
        timeResolutionDiamonds45 = cms.string("2*((x<16)*(-0.171784+0.175856*x-0.0322344*x^2+0.00231489*x^3-5.7575E-05*x^4)+(x>=16)*0.105)"),
        timeResolutionDiamonds56 = cms.string("2*((x<16)*(-0.014943+0.102806*x-0.0209404*x^2+0.00158264*x^3-4.08241E-05*x^4)+(x>=16)*0.089)")
    )
)
