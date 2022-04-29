import FWCore.ParameterSet.Config as cms

# configuration for composite source of alignment, optics, ...
from CalibPPS.ESProducers.ctppsCompositeESSource_cfi import *
ctppsCompositeESSource.generateEveryNEvents = 100

profile_base = cms.PSet(
  L_int = cms.double(1),

  # LHCInfo
  ctppsLHCInfo = cms.PSet(
    xangle = cms.double(-1),
    betaStar = cms.double(-1),
  	beamEnergy  =  cms.double(0),
  	xangleBetaStarHistogramFile = cms.string("CalibPPS/ESProducers/data/xangle_beta_distributions/version1.root"),
  	xangleBetaStarHistogramObject = cms.string("")
  ),

  # optics
  ctppsOpticalFunctions = cms.PSet(
  	opticalFunctions = cms.VPSet(),
  	scoringPlanes = cms.VPSet()
  ),

  # alignment
  ctppsRPAlignmentCorrectionsDataXML = cms.PSet(
    MeasuredFiles = cms.vstring(),
    RealFiles = cms.vstring(),
    MisalignedFiles = cms.vstring()
  ),

  # direct simu data
  ctppsDirectSimuData = cms.PSet(
    empiricalAperture45 = cms.string(""),
    empiricalAperture56 = cms.string(""),

    timeResolutionDiamonds45 = cms.string("999"),
    timeResolutionDiamonds56 = cms.string("999"),

    efficienciesPerRP = cms.VPSet(),
    efficienciesPerPlane = cms.VPSet()
  )
)

# particle-data table
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

# random seeds
RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
  sourceSeed = cms.PSet(initialSeed = cms.untracked.uint32(98765)),
  generator = cms.PSet(initialSeed = cms.untracked.uint32(98766))
)

# default source
source = cms.Source("EmptySource",
  firstRun = cms.untracked.uint32(1),
  numberEventsInLuminosityBlock = ctppsCompositeESSource.generateEveryNEvents
)

# particle generator
from Configuration.Generator.randomXiThetaGunProducer_cfi import *
generator.xi_max = 0.25
generator.theta_x_sigma = 60E-6
generator.theta_y_sigma = 60E-6

# Local track reconstruction 
from RecoPPS.Local.ctppsLocalTrackLiteProducer_cff import ctppsLocalTrackLiteProducer

#----------------------------------------------------------------------------------------------------
# utility functions

def SetSmearingLevel1(obj):
  obj.vtxStddevX = 0E-4
  obj.vtxStddevZ = 0

  obj.beamDivX45 = 0E-6
  obj.beamDivX56 = 0E-6
  obj.beamDivY45 = 0E-6
  obj.beamDivY56 = 0E-6

def SetLevel1(process):
  SetSmearingLevel1(process.ctppsBeamParametersFromLHCInfoESSource)

  process.ppsDirectProtonSimulation.roundToPitch = False

def SetSmearingLevel2(obj):
  obj.beamDivX45 = 0E-6
  obj.beamDivX56 = 0E-6
  obj.beamDivY45 = 0E-6
  obj.beamDivY56 = 0E-6

def SetLevel2(process):
  SetSmearingLevel2(process.ctppsBeamParametersFromLHCInfoESSource)

  process.ppsDirectProtonSimulation.roundToPitch = False

def SetLevel3(process):
  process.ppsDirectProtonSimulation.roundToPitch = False

def SetLevel4(process):
  pass

def SetLowTheta(process):
  process.generator.theta_x_sigma = 0E-6
  process.generator.theta_y_sigma = 0E-6

def SetLargeTheta(process):
  pass

def UseConstantXangleBetaStar(process, xangle, betaStar):
  for p in ctppsCompositeESSource.periods:
    p.ctppsLHCInfo.xangle = xangle
    p.ctppsLHCInfo.betaStar = betaStar

def UseXangleBetaStarHistogram(process, f="", obj=""):
  for p in ctppsCompositeESSource.periods:
    p.ctppsLHCInfo.xangle = -1 # negative value indicates to use the xangle/beta* histogram

    if f:
      p.ctppsLHCInfo.xangleBetaStarHistogramFile = f
    if obj:
      p.ctppsLHCInfo.xangleBetaStarHistogramObject = obj