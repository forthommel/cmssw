import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# geometry
# TODO
#process.load("Geometry.VeryForwardGeometry.geometryRP_cfi")
process.load("Configuration.Geometry.geometry_CTPPS_alaTotem_RECO_cfi")

# load alignment correction
process.load("Geometry.VeryForwardGeometryBuilder.ctppsIncludeAlignments_cfi")
process.ctppsIncludeAlignments.RealFiles = cms.vstring()

# no events to process
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.ctppsGeometryInfo = cms.EDAnalyzer("CTPPSGeometryInfo",
    geometryType = cms.untracked.string("real"),
    printRPInfo = cms.untracked.bool(True),
    printSensorInfo = cms.untracked.bool(False) # TODO: revert to true
)

process.p = cms.Path(
    process.ctppsGeometryInfo
)
