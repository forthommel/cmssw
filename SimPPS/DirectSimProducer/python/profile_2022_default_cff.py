import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2022

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/alignment_2022.xml")

profile_2022_default = profile_base_2022.clone(
    L_int = cms.double(1.),
    ctppsLHCInfo = profile_base_2022.ctppsLHCInfo.clone(
        # NB: until a dedicated 2022 distributions are issued, it is OK to use 2021 ones here
        xangleBetaStarHistogramObject = cms.string("2021/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2022.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2022.ctppsDirectSimuData.clone(
        timeResolutionDiamonds45 = cms.string("0.200"),
        timeResolutionDiamonds56 = cms.string("0.200")
    )
)
