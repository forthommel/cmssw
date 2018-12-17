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
    input = cms.untracked.int32(10)
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
process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test_2017/RP_Dist_Beam_Cent.xml")

# beam-smearing settings
process.load("IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi")
process.beamDivergenceVtxGenerator.src = cms.InputTag("generator", "unsmeared")

process.beamDivergenceVtxGenerator.simulateBeamDivergence = True
process.beamDivergenceVtxGenerator.simulateVertex = True

# values in rad
process.beamDivergenceVtxGenerator.beamDivergenceX = 20E-6
process.beamDivergenceVtxGenerator.beamDivergenceY = 20E-6

# values in cm
process.beamDivergenceVtxGenerator.vertexMeanX = 0.
process.beamDivergenceVtxGenerator.vertexMeanY = 0.
process.beamDivergenceVtxGenerator.vertexMeanZ = 0.

process.beamDivergenceVtxGenerator.vertexSigmaX = 10E-4
process.beamDivergenceVtxGenerator.vertexSigmaY = 10E-4
process.beamDivergenceVtxGenerator.vertexSigmaZ = 5

# fast simulation
process.load('SimCTPPS.OpticsParameterisation.year_2017_OF.ctppsFastProtonSimulation_cfi')
process.ctppsFastProtonSimulation.hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')
process.ctppsFastProtonSimulation.checkApertures = False
process.ctppsFastProtonSimulation.roundToPitch = True
process.ctppsFastProtonSimulation.pitchStrips = 66E-3 * 12 / 19 # effective value to reproduce real RP resolution
process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = True
process.ctppsFastProtonSimulation.produceScoringPlaneHits = False
process.ctppsFastProtonSimulation.produceRecHits = True

for dp in process.ctppsFastProtonSimulation.detectorPackages:
  dp.maxX = 100 # effectively remove high-x cut

# strips reco: pattern recognition
process.load('RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi')
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsFastProtonSimulation')

# strips reco: track fitting
process.load('RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi')

# pixels: local tracks
process.load('RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi')
process.ctppsPixelLocalTracks.label = "ctppsFastProtonSimulation"

# common reco: lite track production
process.load('RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff')
process.ctppsLocalTrackLiteProducer.includeDiamonds = False
process.ctppsLocalTrackLiteProducer.includePixels = True

# proton reconstruction
process.load("RecoCTPPS.ProtonReconstruction.year_2017_OF.ctppsProtonReconstructionOF_cfi")
process.ctppsProtonReconstructionOF.tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer')
process.ctppsProtonReconstructionOF.applyExperimentalAlignment = False
process.ctppsProtonReconstructionOF.xangle = 140  # default value in simulation config

# reconstruction plotter
process.ctppsProtonReconstructionPlotter = cms.EDAnalyzer("CTPPSProtonReconstructionPlotter",
    tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),
    tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOF"),

    rpId_45_F = cms.uint32(3),
    rpId_45_N = cms.uint32(2),
    rpId_56_N = cms.uint32(102),
    rpId_56_F = cms.uint32(103),

    outputFile = cms.string("output.root")
)

# reconstruction validation
process.load("Validation.CTPPS.ctppsProtonReconstructionValidator_cfi")
process.ctppsProtonReconstructionValidator.tagHepMCBeforeSmearing = cms.InputTag("generator", "unsmeared")
process.ctppsProtonReconstructionValidator.tagHepMCAfterSmearing = cms.InputTag("beamDivergenceVtxGenerator")
process.ctppsProtonReconstructionValidator.tagRecoProtons = cms.InputTag("ctppsProtonReconstructionOF")

# processing path
process.p = cms.Path(
    process.generator
    * process.beamDivergenceVtxGenerator
    * process.ctppsFastProtonSimulation

    * process.totemRPUVPatternFinder
    * process.totemRPLocalTrackFitter
    * process.ctppsPixelLocalTracks
    * process.ctppsLocalTrackLiteProducer

    * process.ctppsProtonReconstructionOF
    * process.ctppsProtonReconstructionPlotter
    * process.ctppsProtonReconstructionValidator
)

#----------------------------------------------------------------------------------------------------

# customisation functions

def SetLevel1():
  process.beamDivergenceVtxGenerator.vertexSigmaX = 0E-4
  process.beamDivergenceVtxGenerator.vertexSigmaZ = 0

  process.beamDivergenceVtxGenerator.beamDivergenceX = 0E-6
  process.beamDivergenceVtxGenerator.beamDivergenceY = 0E-6

  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel2():
  process.beamDivergenceVtxGenerator.beamDivergenceX = 0E-6
  process.beamDivergenceVtxGenerator.beamDivergenceY = 0E-6

  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel3():
  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel4():
  pass

def SetLowTheta():
  process.generator.theta_x_sigma = 0E-6
  process.generator.theta_y_sigma = 0E-6


def SetLargeTheta():
  pass

#----------------------------------------------------------------------------------------------------

SetLargeTheta()
SetLevel4()

