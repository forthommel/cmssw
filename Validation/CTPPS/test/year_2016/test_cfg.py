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

# load optics
process.load("CondFormats.CTPPSReadoutObjects.year_2016.ctppsOpticalFunctionsESSource_cfi")

# particle generator
process.generator = cms.EDProducer("RandomXiThetaGunProducer",
  particleId = cms.uint32(2212),

  energy = cms.double(6500),  # nominal beam energy, GeV
  xi_min = cms.double(0.),
  xi_max = cms.double(0.20),
  theta_x_mean = cms.double(0),
  theta_x_sigma = cms.double(50E-6), # in rad
  theta_y_mean = cms.double(0),
  theta_y_sigma = cms.double(50E-6),

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
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")
del(process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test/year_2016/RP_Dist_Beam_Cent.xml")

# TODO
# beam-smearing settings
#process.load("IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi")
#process.beamDivergenceVtxGenerator.src = cms.InputTag("generator", "unsmeared")
#
#process.beamDivergenceVtxGenerator.simulateBeamDivergence = True
#process.beamDivergenceVtxGenerator.simulateVertex = True
#
## values in rad
#process.beamDivergenceVtxGenerator.beamDivergenceX = 20E-6
#process.beamDivergenceVtxGenerator.beamDivergenceY = 20E-6
#
## values in cm
#process.beamDivergenceVtxGenerator.vertexMeanX = 0.
#process.beamDivergenceVtxGenerator.vertexMeanY = 0.
#process.beamDivergenceVtxGenerator.vertexMeanZ = 0.
#
#process.beamDivergenceVtxGenerator.vertexSigmaX = 10E-4
#process.beamDivergenceVtxGenerator.vertexSigmaY = 10E-4
#process.beamDivergenceVtxGenerator.vertexSigmaZ = 5

# fast simulation
process.load('Validation.CTPPS.ctppsDirectProtonSimulation_cfi')
process.ctppsDirectProtonSimulation.verbosity = 0
#process.ctppsDirectProtonSimulation.hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')  # TODO
process.ctppsDirectProtonSimulation.useEmpiricalApertures = False
process.ctppsDirectProtonSimulation.roundToPitch = True
process.ctppsDirectProtonSimulation.pitchStrips = 66E-3 * 12 / 19 # effective value to reproduce real RP resolution
process.ctppsDirectProtonSimulation.produceHitsRelativeToBeam = True
process.ctppsDirectProtonSimulation.produceScoringPlaneHits = False
process.ctppsDirectProtonSimulation.produceRecHits = True

# strips reco: pattern recognition
process.load('RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi')
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsDirectProtonSimulation')

# strips reco: track fitting
process.load('RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi')

# common reco: lite track production
process.load('RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff')
process.ctppsLocalTrackLiteProducer.includeDiamonds = False
process.ctppsLocalTrackLiteProducer.includePixels = False

# distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("output.root")
)

# processing path
process.p = cms.Path(
  process.generator
  #* process.beamDivergenceVtxGenerator # TODO
  * process.ctppsDirectProtonSimulation

  * process.totemRPUVPatternFinder
  * process.totemRPLocalTrackFitter
  * process.ctppsLocalTrackLiteProducer
  * process.ctppsTrackDistributionPlotter
)
