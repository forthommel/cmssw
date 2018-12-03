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

from SimCTPPS.OpticsParameterisation.year_2016.lhcBeamConditions_cff import lhcBeamConditions

# plotter for functions in sector 45
process.ctppsTestOpticalFunctions_45 = cms.EDAnalyzer("CTPPSOpticalFunctionsTester",
    opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root"),
    opticsObjects = cms.vstring("ip5_to_station_150_h_1_lhcb2", "ip5_to_station_150_h_2_lhcb2"),
    beamConditions = lhcBeamConditions,
    verbosity = cms.uint32(0),
    useExtraFunctions = cms.bool(True),
    n_events = cms.uint32(10000),
    outputFile = cms.string("test_45_extra.root")
)

# plotter for functions in sector 56
process.ctppsTestOpticalFunctions_56 = process.ctppsTestOpticalFunctions_45.clone(
    opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root"),
    opticsObjects = cms.vstring("ip5_to_station_150_h_1_lhcb1", "ip5_to_station_150_h_2_lhcb1"),
    outputFile = cms.string("test_56_extra.root")
)

process.p = cms.Path(
    process.ctppsTestOpticalFunctions_45
    * process.ctppsTestOpticalFunctions_56
)
