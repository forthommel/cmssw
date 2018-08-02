import FWCore.ParameterSet.Config as cms

from SimCTPPS.OpticsParameterisation.year_2017.ctppsDetectorPackages_cff import detectorPackages
from SimCTPPS.OpticsParameterisation.year_2017.lhcBeamConditions_cff import lhcBeamConditions

ctppsFastProtonSimulation = cms.EDProducer('CTPPSFastProtonSimulation',
    verbosity = cms.untracked.uint32(0),

    hepMCTag = cms.InputTag('generator', 'unsmeared'),

    beamConditions = lhcBeamConditions,

    # TODO: update this
    opticsFileBeam1 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root'),
    opticsFileBeam2 = cms.FileInPath('CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root'),

    detectorPackages = detectorPackages,

    produceScoringPlaneHits = cms.bool(True),
    produceRecHits = cms.bool(True),

    produceHitsRelativeToBeam = cms.bool(False),

    checkApertures = cms.bool(True),

    roundToPitch = cms.bool(False),
    checkIsHit = cms.bool(True),

    pitchStrips = cms.double(66e-3), # mm
    insensitiveMarginStrips = cms.double(34e-3), # mm

    pitchPixelsHor = cms.double(100e-3), # mm, x in local coordinates
    pitchPixelsVer = cms.double(150e-3), # mm, y in local coordinates
)
