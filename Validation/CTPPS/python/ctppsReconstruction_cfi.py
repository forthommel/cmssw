import FWCore.ParameterSet.Config as cms

from SimRomanPot.CTPPSOpticsParameterisation.ctppsDetectorPackages_cff import detectorPackages_2016PreTS2
from SimRomanPot.CTPPSOpticsParameterisation.lhcBeamConditions_cff import lhcBeamConditions_2016PreTS2

ctppsReconstruction = cms.EDAnalyzer('CTPPSReconstruction',
    genPartTag = cms.InputTag('prunedGenParticles'), # miniAOD
    potsTracksTag = cms.InputTag('ctppsOpticsParameterisation'),
    detectorPackages = detectorPackages_2016PreTS2,
    beamConditions = lhcBeamConditions_2016PreTS2,
)
