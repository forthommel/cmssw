import FWCore.ParameterSet.Config as cms

from SimPPS.DirectSimProducer.profile_base_cff import profile_base as _base
from CalibPPS.ESProducers.ctppsOpticalFunctions_non_DB_cff import optics_2022 as selected_optics

# base profile settings for 2022
profile_base_2022 = _base.clone(
    ctppsLHCInfo = _base.ctppsLHCInfo.clone(
        beamEnergy = 7000
    ),
    ctppsOpticalFunctions = _base.ctppsOpticalFunctions.clone(
        opticalFunctions = selected_optics.opticalFunctions,
        scoringPlanes = selected_optics.scoringPlanes,
    ),
    ctppsDirectSimuData = _base.ctppsDirectSimuData.clone(
        empiricalAperture45 = cms.string("1E3*([xi] - 0.20)"),
        empiricalAperture56 = cms.string("1E3*([xi] - 0.20)")
    )
)

# adjust basic settings
generator.energy = profile_base_2022.ctppsLHCInfo.beamEnergy

from Geometry.VeryForwardGeometry.geometryRPFromDD_2022_cfi import *
ctppsCompositeESSource.compactViewTag = ctppsGeometryESModule.compactViewTag
del ctppsGeometryESModule # this functionality is replaced by the composite ES source

# local reconstruction
ctppsLocalTrackLiteProducer.includeStrips = False
ctppsLocalTrackLiteProducer.includePixels = True
ctppsLocalTrackLiteProducer.includeDiamonds = True

reco_local = cms.Sequence(
  ctppsPixelLocalTracks
  * ctppsDiamondLocalReconstruction
  * ctppsLocalTrackLiteProducer
)

# RP ids
rpIds = cms.PSet(
  rp_45_F = cms.uint32(23),
  rp_45_N = cms.uint32(3),
  rp_56_N = cms.uint32(103),
  rp_56_F = cms.uint32(123)
)

# default list of profiles
from SimPPS.DirectSimProducer.profile_2022_default_cff import profile_2022_default
ctppsCompositeESSource.periods = [profile_2022_default]
