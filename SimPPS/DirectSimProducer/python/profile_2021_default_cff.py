import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2021

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2021.xml")

profile_2021_default = profile_base_2021.clone(
    L_int = cms.double(1.),
    ctppsLHCInfo = profile_base_2021.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2021/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2021.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2021.ctppsDirectSimuData.clone(
        timeResolutionDiamonds45 = cms.string("0.200"),
        timeResolutionDiamonds56 = cms.string("0.200")
    )
)
