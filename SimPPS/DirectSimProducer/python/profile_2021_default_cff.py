import FWCore.ParameterSet.Config as cms

from SimPPS.DirectSimProducer.year_2021_cff import profile_base_2021

alignmentFile = "Validation/CTPPS/alignment/2021.xml"

profile_2021_default = profile_base_2021.clone(
  L_int = 1,

  ctppsLHCInfo = dict(
    xangleBetaStarHistogramObject = "2021/h2_betaStar_vs_xangle"
  ),

  ctppsRPAlignmentCorrectionsDataXML = dict(
    MisalignedFiles = [alignmentFile],
    RealFiles = [alignmentFile]
  ),

  ctppsDirectSimuData = dict(
    timeResolutionDiamonds45 = "0.200",
    timeResolutionDiamonds56 = "0.200"
  )
)
