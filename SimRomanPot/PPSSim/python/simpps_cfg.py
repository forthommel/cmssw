import FWCore.ParameterSet.Config as cms
import math
import sys
import random

NEvents = 0 #number of events to simulate
det1 = 0.   #position of first tracker detector
tof  = 0.   #position of time of flight detector
phys_process = ""
OutputFile = ""
InputFile = ""
min_mass = 100
max_mass = 1000
higgs_decay = 0
higgs_mass  = 125.7
#phi_min = -math.pi
#phi_max =  math.pi
phi_min = 0.
phi_max = 0.
t_min   = 0.
t_max   = 2.
xi_min  = 0.
xi_max  = 0.2

ecms = 13000.

for arg in sys.argv[2:]:
    opt = arg[0:arg.find("=")]
    if opt=="process":
         phys_process = arg[arg.find("=")+1:]
         if phys_process!="QQ" and phys_process!="Higgs" and phys_process!="GG" and phys_process!="gg" and phys_process!="DiPhoton" and phys_process!="PG" and phys_process!="WW":
            print "Invalid physics process"
            sys.exit(2)
    elif opt.lower()=="nevents":
         NEvents = int(arg[arg.find("=")+1:])
    elif opt=="output":
         OutputFile = arg[arg.find("=")+1:]
    elif opt=="input":
         InputFile = arg[arg.find("=")+1:]
    elif opt=="det1":
         det1=float(arg[arg.find("=")+1:])
    elif opt=="tof":
         tof=float(arg[arg.find("=")+1:])
    elif opt=="min_mass":
         min_mass=float(arg[arg.find("=")+1:])
    elif opt=="max_mass":
         max_mass=float(arg[arg.find("=")+1:])
    elif opt=="hdecay":
         hdecay=arg[arg.find("=")+1:]
         if hdecay=="b":
            higgs_decay = 5
         elif hdecay=="c":
            higgs_decay = 4
         elif hdecay=="W":
            higgs_decay = 24
         elif hdecay=="G":
            higgs_decay = 21
         elif hdecay=="tau":
            higgs_decay = 15
         elif hdecay=="Z":
            higgs_decay = 23
         elif hdecay=="g":
            higgs_decay = 22
         else:
            print "Unknown decay mode"
            sys.exit(2)
    elif opt=="higgs_mass":
         higgs_mass = float(arg[arg.find("=")+1:])  
    elif opt=="min_t":
         t_min = float(arg[arg.find("=")+1:])
    elif opt=="max_t":
         t_max = float(arg[arg.find("=")+1:])
    elif opt=="min_xi":
         xi_min = float(arg[arg.find("=")+1:])
    elif opt=="max_xi":
         xi_max = float(arg[arg.find("=")+1:])
    elif opt=="ecms":
         ecms = float(arg[arg.find("=")+1:])
    else:
         print opt
         print "Usage: cmsRun ",sys.argv[1],"process=[QQ,GG,DiPhoton]","NEvent=# events","det1=det positon","tof=ToF position"

print "Number of events : ",NEvents
print "Physics process  : ",phys_process
print "sqrt(s)          : ",ecms
print "Detector position: ",det1
print "ToF det. position: ",tof
print "Minimum mass     : ",min_mass
print "Maximum mass     : ",max_mass
print "Minimum t        : ",t_min
print "Maximum t        : ",t_max
print "Minimum xi       : ",xi_min
print "Maximum xi       : ",xi_max
print "Minimum phi      : ",phi_min
print "Maximum phi      : ",phi_max
print "Higgs mass       : ",higgs_mass
print "Higgs decay      : ",higgs_decay
print "Output File      : ",OutputFile

if (NEvents==0 and phys_process!="WW" and InputFile=="") or phys_process=="" or OutputFile=="" or det1==0 or tof==0 or (phys_process=="H" and higgs_decay==0):
   print
   print "Usage: cmsRun ",sys.argv[1],"process=[QQ,GG,DiPhoton]","NEvent=# events","det1=det positon","tof=ToF position","output=file.root"
   print "Usage: cmsRun ",sys.argv[1],"process=[WW]","input=file.lhe","det1=det positon","tof=ToF position","output=file.root"
   print
   print
   sys.exit(2)

if phys_process=="Higgs":
   min_mass = 120.
   max_mass = 130.

process = cms.Process("PPS")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('IOMC.EventVertexGenerators.VtxSmearedGauss_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")
#process.RandomNumberGeneratorService.simSiPixelDigis.initialSeed = random.randint(0,900000000)
#process.RandomNumberGeneratorService.simSiPixelDigis.engineName = cms.untracked.string('TRandom3')
process.RandomNumberGeneratorService.g4SimHits.initialSeed  = cms.untracked.uint32(random.randint(0,900000000))
process.RandomNumberGeneratorService.VtxSmeared.initialSeed = cms.untracked.uint32(random.randint(0,900000000))
process.RandomNumberGeneratorService.generator.initialSeed  = cms.untracked.uint32(random.randint(0,900000000))


process.VtxSmeared.MeanX    = cms.double(0.0)
process.VtxSmeared.MeanY    = cms.double(0.0)
process.VtxSmeared.MeanZ    = cms.double(0.0)
process.VtxSmeared.SigmaX   = cms.double(0.0015)
process.VtxSmeared.SigmaY   = cms.double(0.0015)
process.VtxSmeared.SigmaZ   = cms.double(5.3)
# import of standard configurations
from GeneratorInterface.ExhumeInterface.ExhumeParameters_cfi import ExhumeParameters as ExhumeParametersRef
ExhumeParametersRef.HiggsMass = cms.double(higgs_mass)



if phys_process=="PG":
   process.generator = cms.EDProducer("RandomtXiGunProducer",
       PGunParameters = cms.PSet(
           PartID = cms.vint32(2212),
           MinPhi = cms.double(phi_min),
           MaxPhi = cms.double(phi_max),
           ECMS   = cms.double(ecms),
           Mint   = cms.double(t_min),
           Maxt   = cms.double(t_max),
           MinXi  = cms.double(xi_min),
           MaxXi  = cms.double(xi_max)
       ),
       Verbosity = cms.untracked.int32(0),
       psethack = cms.string('single protons'),
       FireBackward = cms.bool(True),
       FireForward  = cms.bool(True),
       firstRun = cms.untracked.uint32(1)
   )
   process.source = cms.Source("EmptySource")
elif phys_process=="QQ" or phys_process=="GG" or phys_process=="DiPhoton":
   process.generator = cms.EDFilter("ExhumeGeneratorFilter",
       PythiaParameters = cms.PSet(
          parameterSets = cms.vstring()
       ),
       ExhumeParameters = ExhumeParametersRef,
       comEnergy = cms.double(ecms),
       pythiaHepMCVerbosity = cms.untracked.bool(False),
       maxEventsToPrint = cms.untracked.int32(2),
       pythiaPylistVerbosity = cms.untracked.int32(1),
       ExhumeProcess = cms.PSet(
           MassRangeLow = cms.double(min_mass),
           MassRangeHigh = cms.double(max_mass),
           ProcessType = cms.string(phys_process),
           ThetaMin = cms.double(0.30),
           HiggsMass = cms.double(higgs_mass),
           HiggsDecay = cms.int32(higgs_decay),
           QuarkType  = cms.int32(5)
       )
   )
   process.source = cms.Source("EmptySource")
elif phys_process=="WW":
   process.source = cms.Source("LHESource",
                    fileNames=cms.untracked.vstring(
                              'file:'+InputFile
                           )
                 )
   from GeneratorInterface.ExternalDecays.TauolaSettings_cff import *

   process.generator = cms.EDFilter("Pythia6HadronizerFilter",
       pythiaHepMCVerbosity = cms.untracked.bool(False),
       maxEventsToPrint = cms.untracked.int32(0),
       pythiaPylistVerbosity = cms.untracked.int32(0),
       comEnergy = cms.double(13000.0),
       ExternalDecays = cms.PSet(
                   Tauola = cms.untracked.PSet(
                           TauolaPolar,
                           TauolaDefaultInputCards
                   ),
                   parameterSets = cms.vstring('Tauola')
       ),
       UseExternalGenerators = cms.untracked.bool(True),
   
       PythiaParameters = cms.PSet(
           pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function',
               'MSTJ(22)=2     ! Decay those unstable particles',
               'PARJ(71)=10 .  ! for which ctau  10 mm',
               'MSTP(2)=1      ! which order running alphaS',
               'MSTP(33)=0     ! no K factors in hard cross sections',
               'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)',
               'MSTP(52)=2     ! work with LHAPDF',
               'MSTP(81)=0     ! multiple parton interactions 1 is Pythia default',
               'MSTP(82)=4     ! Defines the multi-parton model',
               'MSTU(21)=1     ! Check on possible errors during program execution',
               'PARP(82)=1.8387   ! pt cutoff for multiparton interactions',
               'PARP(89)=1960. ! sqrts for which PARP82 is set',
               'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter',
               'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter',
               'PARP(90)=0.16  ! Multiple interactions: rescaling power',
               'PARP(67)=2.5    ! amount of initial-state radiation',
               'PARP(85)=1.0  ! gluon prod. mechanism in MI',
               'PARP(86)=1.0  ! gluon prod. mechanism in MI',
               'PARP(62)=1.25   ! ',
               'PARP(64)=0.2    ! ',
               'MSTP(91)=1      !',
               'PARP(91)=2.1   ! kt distribution',
               'PARP(93)=15.0  ! '),
           processParameters = cms.vstring('MSEL=0         ! User defined processes',
               'MDME(190,1) = 0    !W decay into dbar u',
               'MDME(191,1) = 0    !W decay into dbar c',
               'MDME(192,1) = 0    !W decay into dbar t',
               'MDME(194,1) = 0    !W decay into sbar u',
               'MDME(195,1) = 0    !W decay into sbar c',
               'MDME(196,1) = 0    !W decay into sbar t',
               'MDME(198,1) = 0    !W decay into bbar u',
               'MDME(199,1) = 0    !W decay into bbar c',
               'MDME(200,1) = 0    !W decay into bbar t',
               'MDME(205,1) = 0    !W decay into bbar tp',
               'MDME(206,1) = 1    !W decay into e+ nu_e',
               'MDME(207,1) = 1    !W decay into mu+ nu_mu',
               'MDME(208,1) = 0    !W decay into tau+ nu_tau',
               'PMAS(5,1)=4.4   ! b quark mass',
               'PMAS(6,1)=172.4 ! t quark mass',
               'MSTJ(1)=0       ! Fragmentation/hadronization on or off',
               'MSTP(61)=0      ! Parton showering on or off',
               'MSTP(98)=1      ! Elastic two-photon'),
           parameterSets = cms.vstring('pythiaUESettings','processParameters')
       )
)

process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEvents) )


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.398 $'),
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

ppssim_options = cms.PSet(
                         Verbosity = cms.untracked.int32(0),
                         Beam1File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB1_Beta0.60_6.5TeV_v6.503.tfs'),
                         Beam2File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB2_Beta0.60_6.5TeV_v6.503.tfs'),
                         #Beam1File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB1_Beta0.60_7.0TeV_v6.503.tfs'),
                         #Beam2File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB2_Beta0.60_7.0TeV_v6.503.tfs'),
                         #Beam1File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB1IR5_v6.500.tfs'),
                         #Beam2File = cms.string('/afs/cern.ch/work/m/mundim/public/PPS/CMSSW_5_0_0/src/data/LHCB2IR5_v6.500.tfs'),
                         Beam1Direction = cms.int32(1),
                         Beam2Direction = cms.int32(1),
                         SmearEnergy    = cms.bool(False),
                         BeamEnergyRMS  = cms.double(1.11e-4),
                         SmearAngle     = cms.bool(False),
                         BeamAngleRMS   = cms.double(30.03), # in mrad
                         ShowBeamLine   = cms.untracked.bool(False),
                         SimBeamProfile = cms.untracked.bool(False),
                         CollisionPoint = cms.string("IP5"),
                         TrackerPosition = cms.double(det1),
                         TrackerLength   = cms.double(10.),
                         ToFPosition     = cms.double(tof),
                         TCL4Position    = cms.untracked.double(143.0),
                         #TCL5Position    = cms.untracked.double(256.7),
                         TCL5Position    = cms.untracked.double(183.8),
                         SmearHit        = cms.bool(False),
                         HitSigmaX       = cms.double(10),
                         HitSigmaY       = cms.double(10),
                         HitSigmaZ       = cms.double(0),
                         TimeSigma       = cms.double(0.01), #in ns
                         PhiMin          = cms.double(phi_min),
                         PhiMax          = cms.double(phi_max),
                         CentralMass     = cms.double(125.7),
                         CentralMassErr  = cms.double(0.4),
                         KickersOFF      = cms.bool(True),
                         CrossAngleCorr  = cms.bool(False),
                         CrossingAngle   = cms.double(142.5) #in mrad
                     )

if phys_process=="PG":
    process.ppssim = cms.EDAnalyzer('PPSAnalyzer',
                         ppssim_options,
                         OutputFile= cms.string(OutputFile)
                     )
else:
    process.ppssim = cms.EDProducer('PPSProducer',
                         ppssim_options
                     )

# Output definition
if phys_process=="PG":
    process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
            splitLevel = cms.untracked.int32(0),
            eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
            outputCommands = cms.untracked.vstring('drop *',
                            'keep PPSSpectrometer_*_*_*'
            ),
            fileName = cms.untracked.string(OutputFile),
            dataset = cms.untracked.PSet(
                      filterName = cms.untracked.string(''),
                      dataTier = cms.untracked.string('')
            )
    )
else:
    process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
            splitLevel = cms.untracked.int32(0),
            eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
            outputCommands = cms.untracked.vstring('drop *',
                             'keep edmHepMCProduct_*_*_*',
                             'keep SimTracks_*_*_*',
                             'keep SimVertexs_*_*_*',
                             'keep recoCaloJets_*_*_*',
                             'keep recoPFCandidates_*_*_*',
                             'keep recoPFJets_*_*_*',
                             'keep PPSSpectrometer_*_*_*',
                             'keep recoTracks_*_*_*'),
            fileName = cms.untracked.string(OutputFile),
            dataset = cms.untracked.PSet(
                      filterName = cms.untracked.string(''),
                      dataTier = cms.untracked.string('')
            )
    )

# Other statements
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')
process.GlobalTag.globaltag = 'START50_V16::All'

# Path and EndPath definitions
process.generation_step = cms.Path(process.generator)
process.vtxsmearing_step = cms.Path(process.VtxSmeared)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.ppssim_step = cms.Path(process.ppssim)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
if phys_process=="PG":
   process.schedule = cms.Schedule(process.generation_step,process.vtxsmearing_step,
                                   process.ppssim_step
   )
#elif phys_process=="WW":
#   process.schedule = cms.Schedule(process.ppssim_step)
else:
   process.schedule = cms.Schedule(process.generation_step,
                                   process.genfiltersummary_step,
                                   process.vtxsmearing_step,
                                   process.simulation_step,
                                   process.digitisation_step,
                                   process.L1simulation_step,
                                   process.digi2raw_step,
                                   process.raw2digi_step,
                                   process.reconstruction_step,
                                   process.ppssim_step,
                                   process.endjob_step,
                                   process.RECOSIMoutput_step
   )
