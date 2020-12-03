import FWCore.ParameterSet.Config as cms
from Geometry.VeryForwardGeometryBuilder.ctppsGeometryESModule_cfi import ctppsGeometryESModule as _ctppsGeometryESModule

DDDetectorESProducer = cms.ESSource("DDDetectorESProducer",
    confGeomXMLFiles = cms.FileInPath('Geometry/VeryForwardGeometry/data/dd4hep/empty.xml'),
    label = cms.string('CTPPS'),
    fromDB = cms.bool(True),
    appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

DDCompactViewESProducer = cms.ESProducer("DDCompactViewESProducer",
    appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
)

ctppsGeometryESModule = _ctppsGeometryESModule.clone(
    fromDD4hep = cms.untracked.bool(True),
    compactViewTag = cms.string('XMLIdealGeometryESSource_CTPPS')
)

