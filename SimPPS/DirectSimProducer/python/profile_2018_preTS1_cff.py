import FWCore.ParameterSet.Config as cms
from SimPPS.DirectSimProducer.profile_base_cff import profile_base_2018

_alignmentFiles = cms.vstring("Validation/CTPPS/alignment/2018_preTS1.xml")

profile_2018_preTS1 = profile_base_2018.clone(
    L_int = cms.double(18.488297964),
    ctppsLHCInfo = profile_base_2018.ctppsLHCInfo.clone(
        xangleBetaStarHistogramObject = cms.string("2018_preTS1/h2_betaStar_vs_xangle")
    ),
    ctppsRPAlignmentCorrectionsDataXML = profile_base_2018.ctppsRPAlignmentCorrectionsDataXML.clone(
        MisalignedFiles = _alignmentFiles,
        RealFiles = _alignmentFiles
    ),
    ctppsDirectSimuData = profile_base_2018.ctppsDirectSimuData.clone(
        # timing not available in this period
        timeResolutionDiamonds45 = cms.string("0.200"),
        timeResolutionDiamonds56 = cms.string("0.200")
    )
)
