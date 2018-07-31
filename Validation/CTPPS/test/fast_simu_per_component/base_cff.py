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
    input = cms.untracked.int32(1)
)

# particle-data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# particle generator
process.generator = cms.EDProducer("RandomXiThetaGunProducer",
  particleId = cms.uint32(2212),

  energy = cms.double(6500),  # nominal beam energy, GeV
  xi_min = cms.double(0.1),
  xi_max = cms.double(0.1),
  theta_x_mean = cms.double(0),
  theta_x_sigma = cms.double(0E-6), # in rad
  theta_y_mean = cms.double(0),
  theta_y_sigma = cms.double(0E-6),

  nParticlesSector45 = cms.uint32(1),
  nParticlesSector56 = cms.uint32(1),
)

# random seeds
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.PSet(initialSeed =cms.untracked.uint32(98765)),
    generator = cms.PSet(initialSeed = cms.untracked.uint32(98766)),
    beamDivergenceVtxGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
)

# geometry
from Geometry.VeryForwardGeometry.geometryRP_cfi import totemGeomXMLFiles, ctppsDiamondGeomXMLFiles

process.XMLIdealGeometryESSource_CTPPS = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = totemGeomXMLFiles+ctppsDiamondGeomXMLFiles+["Validation/CTPPS/test/fast_simu_with_phys_generator/qgsjet/global/RP_Dist_Beam_Cent.xml"],
    rootNodeName = cms.string('cms:CMSE'),
)

process.TotemRPGeometryESModule = cms.ESProducer("TotemRPGeometryESModule")

# beam-smearing settings
process.load("IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi")
process.beamDivergenceVtxGenerator.src = cms.InputTag("generator", "unsmeared")

process.beamDivergenceVtxGenerator.simulateBeamDivergence = True
process.beamDivergenceVtxGenerator.simulateVertex = True

# values in rad
process.beamDivergenceVtxGenerator.beamDivergenceX = 0E-6
process.beamDivergenceVtxGenerator.beamDivergenceY = 0E-6

# values in cm
process.beamDivergenceVtxGenerator.vertexMeanX = 0.
process.beamDivergenceVtxGenerator.vertexMeanY = 0.
process.beamDivergenceVtxGenerator.vertexMeanZ = 0.

process.beamDivergenceVtxGenerator.vertexSigmaX = 0E-4
process.beamDivergenceVtxGenerator.vertexSigmaY = 0E-4
process.beamDivergenceVtxGenerator.vertexSigmaZ = 0

# fast simulation
process.load('SimCTPPS.OpticsParameterisation.ctppsFastProtonSimulation_cfi')
process.ctppsFastProtonSimulation.hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')
process.ctppsFastProtonSimulation.checkApertures = True
process.ctppsFastProtonSimulation.roundToPitch = False
process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = True
process.ctppsFastProtonSimulation.produceScoringPlaneHits = True
process.ctppsFastProtonSimulation.produceRecHits = False

# distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tracksTag = cms.InputTag("ctppsFastProtonSimulation"),
  outputFile = cms.string("output.root")
)

# proton reconstruction
process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.tagLocalTrackLite = cms.InputTag('ctppsFastProtonSimulation')

# processing path
process.p = cms.Path(
    process.generator
    * process.beamDivergenceVtxGenerator
    * process.ctppsFastProtonSimulation

    * process.ctppsTrackDistributionPlotter

    * process.ctppsProtonReconstruction
)
