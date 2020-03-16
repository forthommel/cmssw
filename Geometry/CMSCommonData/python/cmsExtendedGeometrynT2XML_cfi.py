import FWCore.ParameterSet.Config as cms

from Geometry.CMSCommonData.cmsExtendedGeometry2021XML_cfi import XMLIdealGeometryESSource as base

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = base.geomXMLFiles+(
        'Geometry/VeryForwardData/data/TotemT2/nT2_Parameters.xml',
    ),
    rootNodeName = cms.string('cms:OCMS')
)


