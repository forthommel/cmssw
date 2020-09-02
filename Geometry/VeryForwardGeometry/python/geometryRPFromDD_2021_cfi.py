import FWCore.ParameterSet.Config as cms

DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
                                            confGeomXMLFiles = cms.FileInPath('Geometry/VeryForwardGeometry/data/dd4hep/geometryRPFromDD_2021.xml'),
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
                                            appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

ctppsGeometryESModule = cms.ESProducer("PPSGeometryESProducer",
    verbosity = cms.untracked.uint32(1),
    detectorTag = cms.string("XMLIdealGeometryESSource_CTPPS")
)

