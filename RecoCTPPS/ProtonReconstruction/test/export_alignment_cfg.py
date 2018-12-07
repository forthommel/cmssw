import FWCore.ParameterSet.Config as cms
process = cms.Process("GeometryInfo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# no events to process
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(1)
)

# geometry
process.load("Geometry.VeryForwardGeometry.geometryRPFromDD_2017_cfi")

process.load("Geometry.VeryForwardGeometryBuilder.ctppsIncludeAlignmentsFromXML_cfi")

process.ctppsExportAlignment = cms.EDAnalyzer("CTPPSExportAlignment",
  inputFiles = cms.vstring(
    "RecoCTPPS/ProtonReconstruction/data/alignment/2016_preTS2/collect_alignments_2018_07_30.5.out",
    "RecoCTPPS/ProtonReconstruction/data/alignment/2017_preTS2/collect_alignments_2018_10_26.4.out",
    "RecoCTPPS/ProtonReconstruction/data/alignment/2017_postTS2/collect_alignments_2018_10_25.5.out",
    "RecoCTPPS/ProtonReconstruction/data/alignment/2018/collect_alignments_2018_11_02.3.out",
  ),
  outputFile = cms.string("alignment_export_2018_12_07.1.xml")
)

process.p = cms.Path(
    process.ctppsExportAlignment
)
