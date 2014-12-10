import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'Geometry/CMSCommonData/data/rotations.xml', 
        'Geometry/CMSCommonData/data/extend/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMother.xml', 
        'Geometry/PPSCommonData/data/cmsfp420.xml', 
        'Geometry/PPSCommonData/data/fp420.xml', 
        'Geometry/PPSCommonData/data/zzzrectangle.xml', 
        'Geometry/PPSCommonData/data/materialsfp420.xml', 
        'Geometry/PPSCommonData/data/FP420Rot.xml', 
        'Geometry/PPSSimData/data/fp420sens.xml', 
        'Geometry/PPSSimData/data/FP420ProdCuts.xml'),
    rootNodeName = cms.string('cms:OCMS')
)


