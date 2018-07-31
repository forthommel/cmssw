import FWCore.ParameterSet.Config as cms

from SimCTPPS.OpticsParameterisation.ctppsDetectorPackages_cff import detectorPackages_2016PreTS2
from SimCTPPS.OpticsParameterisation.lhcBeamConditions_cff import lhcBeamConditions_2016PreTS2

ctppsProtonReconstruction = cms.EDProducer('CTPPSProtonReconstruction',
    verbosity = cms.untracked.uint32(0),

    tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer'),

    beamConditions = lhcBeamConditions_2016PreTS2,
    detectorPackages = detectorPackages_2016PreTS2,

    checkApertures = cms.bool(True),

    opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root'),
    opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root'),

    applyExperimentalAlignment = cms.bool(False),
    alignmentFile = cms.FileInPath("RecoCTPPS/ProtonReconstruction/data/collect_alignments_2018_07_30.5.out"),

    applyTestAlignment = cms.bool(False),
    de_x_N = cms.double(0.),
    de_x_F = cms.double(0.),
    de_y_N = cms.double(0.),
    de_y_F = cms.double(0.)
)
