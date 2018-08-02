import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('CTPPSFastSimulation', eras.ctpps_2016)

# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# number of events
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# particle-data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# particle generator
process.generator = cms.EDProducer("RandomXiThetaGunProducer",
  particleId = cms.uint32(2212),

  energy = cms.double(6500),  # nominal beam energy, GeV
  xi_min = cms.double(0.),
  xi_max = cms.double(0.20),
  theta_x_mean = cms.double(0),
  theta_x_sigma = cms.double(100E-6), # in rad
  theta_y_mean = cms.double(0),
  theta_y_sigma = cms.double(100E-6),

  nParticlesSector45 = cms.uint32(1),
  nParticlesSector56 = cms.uint32(1),
)

# random seeds
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.PSet(initialSeed =cms.untracked.uint32(98765)),
    generator = cms.PSet(initialSeed = cms.untracked.uint32(98766)),
    SmearingGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")
del(process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test_2017/RP_Dist_Beam_Cent.xml")

# alternative association between RPs and optics approximators
from SimCTPPS.OpticsParameterisation.ctppsDetectorPackages_cff import genericStripsPackage
detectorPackagesLong = cms.VPSet(
    #----- sector 45
    genericStripsPackage.clone(
        potId = cms.uint32(0x76100000), # 002
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb2'),
        zPosition = cms.double(-203.826),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(0x76180000), # 003
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb2'),
        zPosition = cms.double(-203.826),
    ),

    #----- sector 56
    genericStripsPackage.clone(
        potId = cms.uint32(0x77100000), # 102
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb1'),
        zPosition = cms.double(+203.826),
    ),
    genericStripsPackage.clone(
        potId = cms.uint32(0x77180000), # 103
        interpolatorName = cms.string('ip5_to_station_150_h_1_lhcb1'),
        zPosition = cms.double(+203.826),
    ),
)

# fast simulation
from SimCTPPS.OpticsParameterisation.year_2017.ctppsFastProtonSimulation_cfi import ctppsFastProtonSimulation
ctppsFastProtonSimulation.checkApertures = True
ctppsFastProtonSimulation.produceHitsRelativeToBeam = False
ctppsFastProtonSimulation.roundToPitch = False

process.ctppsFastProtonSimulationStd = ctppsFastProtonSimulation.clone(
    produceScoringPlaneHits = cms.bool(True),
    produceRecHits = cms.bool(False)
)

process.ctppsFastProtonSimulationLong = ctppsFastProtonSimulation.clone(
    produceScoringPlaneHits = cms.bool(False),
    produceRecHits = cms.bool(True),
    detectorPackages = detectorPackagesLong
)

# local track reco
process.load('RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi')
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsFastProtonSimulationLong')

process.load('RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi')

process.load("RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi")
process.ctppsPixelLocalTracks.label = "ctppsFastProtonSimulationLong"

process.load('RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff')
process.ctppsLocalTrackLiteProducer.includeDiamonds = False

# distribution plotters
process.ctppsFastSimulationValidator = cms.EDAnalyzer("CTPPSFastSimulationValidator",
  simuTracksTag = cms.InputTag("ctppsFastProtonSimulationStd"),
  recoTracksTag = cms.InputTag("ctppsLocalTrackLiteProducer", ""),
  outputFile = cms.string("output_long_extrapolation.root")
)

# processing path
process.p = cms.Path(
    process.generator

    * process.ctppsFastProtonSimulationStd

    * process.ctppsFastProtonSimulationLong
    * process.totemRPUVPatternFinder
    * process.totemRPLocalTrackFitter
    * process.ctppsPixelLocalTracks
    * process.ctppsLocalTrackLiteProducer

    * process.ctppsFastSimulationValidator
)
