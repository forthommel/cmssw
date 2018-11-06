import FWCore.ParameterSet.Config as cms

from SimCTPPS.OpticsParameterisation.year_2018_OF.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2018_OF.lhcBeamConditions_cff import lhcBeamConditions

ctppsProtonReconstructionOFDB = cms.EDProducer('CTPPSProtonReconstructionOFDB',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    beamConditions = lhcBeamConditions,
    detectorPackages = detectorPackages,

    xangle1 = cms.double(120), # in urad
    opticsFile1 = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_120urad_B1_FAKE.root"), # TODO: update

    xangle2 = cms.double(140),
    opticsFile2 = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2017/optical_functions_2017_140urad.root"),

    applyExperimentalAlignment = cms.bool(True),
    alignmentFiles = cms.vstring(
      "RecoCTPPS/ProtonReconstruction/data/alignment/2018/collect_alignments_2018_11_02.3.out",
    ),

    applyTestAlignment = cms.bool(False),
    de_x_N = cms.double(0.),
    de_x_F = cms.double(0.),
    de_y_N = cms.double(0.),
    de_y_F = cms.double(0.)
)
