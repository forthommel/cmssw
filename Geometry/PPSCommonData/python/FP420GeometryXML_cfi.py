import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/PPSCommonData/data/fp420world.xml', 
        'Geometry/PPSCommonData/data/fp420.xml', 
        'Geometry/PPSCommonData/data/zzzrectangle.xml', 
        'Geometry/PPSCommonData/data/materialsfp420.xml', 
        'Geometry/PPSCommonData/data/FP420Rot.xml', 
        'Geometry/PPSSimData/data/fp420sens.xml', 
        'Geometry/PPSSimData/data/FP420ProdCuts.xml'),
    rootNodeName = cms.string('fp420world:OCMS')
)


