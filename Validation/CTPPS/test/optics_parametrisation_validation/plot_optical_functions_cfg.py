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

# retrieve the two plotters
process.load('Validation.CTPPS.opticalFunctionsPlotter_cfi')

from SimCTPPS.OpticsParameterisation.year_2017.lhcBeamConditions_cff import lhcBeamConditions

# TODO: update
process.ctppsPlotOpticalFunctions_45.opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam2/parametrization_6500GeV_0p4_185_reco.root")
process.ctppsPlotOpticalFunctions_45.opticsObjects = cms.vstring("ip5_to_station_150_h_1_lhcb2", "ip5_to_station_150_h_2_lhcb2")
process.ctppsPlotOpticalFunctions_45.beamConditions = lhcBeamConditions

# TODO: update
process.ctppsPlotOpticalFunctions_45.opticsFile = cms.FileInPath("CondFormats/CTPPSOpticsObjects/data/2016_preTS2/version4-vale1/beam1/parametrization_6500GeV_0p4_185_reco.root")
process.ctppsPlotOpticalFunctions_45.opticsObjects = cms.vstring("ip5_to_station_150_h_1_lhcb1", "ip5_to_station_150_h_2_lhcb1")
process.ctppsPlotOpticalFunctions_45.beamConditions = lhcBeamConditions

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('optical_functions.root'),
    closeFileFast = cms.untracked.bool(True),
)

process.p = cms.Path(
    process.ctppsPlotOpticalFunctions_45
    * process.ctppsPlotOpticalFunctions_56
)
