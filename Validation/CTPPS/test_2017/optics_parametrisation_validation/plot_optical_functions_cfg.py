import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryTest")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# no events to process
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
)

from SimCTPPS.OpticsParameterisation.year_2017.lhcBeamConditions_cff import lhcBeamConditions

lhcBeamConditions.xOffsetSector45 = cms.double(0.) # in m
lhcBeamConditions.xOffsetSector56 = cms.double(0.) # in m

lhcBeamConditions.halfCrossingAngleSector45 = cms.double(140e-6) # in rad
lhcBeamConditions.halfCrossingAngleSector56 = cms.double(140e-6) # in rad

ctppsPlotOpticalFunctions_base = cms.EDAnalyzer("OpticalFunctionsPlotter",
    beamConditions = lhcBeamConditions,
    minXi = cms.double(0.0),
    maxXi = cms.double(0.301),
    xiStep = cms.double(0.001),
)

# plotter for functions in sector 45
#process.ctppsPlotOpticalFunctions_45 = ctppsPlotOpticalFunctions_base.clone(
#    opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root"),
#    opticsObjects = cms.vstring("ip5_to_station_150_h_1_lhcb2", "ip5_to_station_150_h_2_lhcb2"),
#)

# plotter for functions in sector 56
process.ctppsPlotOpticalFunctions_56 = ctppsPlotOpticalFunctions_base.clone(
    opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2017/parametrization_6500GeV_0p4_2017_reco_nominal_2.root"),
    opticsObjects = cms.vstring("ip5_to_station_150_h_2_lhcb1", "ip5_to_station_220_h_2_lhcb1",
      #"ip5_to_tcl4_lhcb1",
      "ip5_to_beg_150_station_lhcb1",
      "ip5_to_beg_220_station_lhcb1",
    ),
)


# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('optical_functions_xangle140_nominal_2_x0.root'),
    closeFileFast = cms.untracked.bool(True),
)

process.p = cms.Path(
    #process.ctppsPlotOpticalFunctions_45
    process.ctppsPlotOpticalFunctions_56
)
