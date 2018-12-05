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
    input = cms.untracked.int32(1000000)
)

# particle-data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# particle generator
process.generator = cms.EDProducer("RandomXiThetaGunProducer",
  particleId = cms.uint32(2212),

  energy = cms.double(6500),  # nominal beam energy, GeV
  xi_min = cms.double(0.),
  xi_max = cms.double(0.25),
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
    SmearingGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")
del(process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles[-1])
process.XMLIdealGeometryESSource_CTPPS.geomXMLFiles.append("Validation/CTPPS/test_2017/RP_Dist_Beam_Cent.xml")

# fast simulation
process.load('SimCTPPS.OpticsParameterisation.year_2017_OF.ctppsFastProtonSimulation_cfi')
process.ctppsFastProtonSimulation.xangle = 140
process.ctppsFastProtonSimulation.produceScoringPlaneHits = False
process.ctppsFastProtonSimulation.produceRecHits = True
process.ctppsFastProtonSimulation.checkApertures = False
process.ctppsFastProtonSimulation.useEmpiricalApertures = True 
process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = True 
process.ctppsFastProtonSimulation.roundToPitch = True

# local track reco
process.load('RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi')
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsFastProtonSimulation')

process.load('RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi')

process.load("RecoCTPPS.PixelLocal.ctppsPixelLocalTracks_cfi")
process.ctppsPixelLocalTracks.label = "ctppsFastProtonSimulation"

process.load('RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cff')
process.ctppsLocalTrackLiteProducer.includeDiamonds = False

# acceptance plotter
process.ctppsAcceptancePlotter = cms.EDAnalyzer("CTPPSAcceptancePlotter",
  tagHepMC = cms.InputTag("generator", "unsmeared"),
  tagTracks = cms.InputTag("ctppsLocalTrackLiteProducer"),

  rpId_45_F = cms.uint32(23),
  rpId_45_N = cms.uint32(3),
  rpId_56_N = cms.uint32(103),
  rpId_56_F = cms.uint32(123),

  outputFile = cms.string("acceptance_xangle_140.root")
)

# processing path
process.p = cms.Path(
    process.generator

    * process.ctppsFastProtonSimulation

    * process.totemRPUVPatternFinder
    * process.totemRPLocalTrackFitter
    * process.ctppsPixelLocalTracks
    * process.ctppsLocalTrackLiteProducer

    * process.ctppsAcceptancePlotter
)

def UseCrossingAngle150():
  process.ctppsFastProtonSimulation.xangle = 150
  process.ctppsFastProtonSimulation.empiricalAperture45_xi0 = 0.158
  process.ctppsFastProtonSimulation.empiricalAperture56_xi0 = 0.20
  process.ctppsAcceptancePlotter.outputFile = "acceptance_xangle_150.root"

def UseCrossingAngle140():
  process.ctppsFastProtonSimulation.xangle = 140
  process.ctppsFastProtonSimulation.empiricalAperture45_xi0 = 0.153
  process.ctppsFastProtonSimulation.empiricalAperture56_xi0 = 0.19
  process.ctppsAcceptancePlotter.outputFile = "acceptance_xangle_140.root"

def UseCrossingAngle130():
  process.ctppsFastProtonSimulation.xangle = 130
  process.ctppsFastProtonSimulation.empiricalAperture45_xi0 = 0.148
  process.ctppsFastProtonSimulation.empiricalAperture56_xi0 = 0.18
  process.ctppsAcceptancePlotter.outputFile = "acceptance_xangle_130.root"

UseCrossingAngle130()
