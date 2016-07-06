import FWCore.ParameterSet.Config as cms

from CondFormats.TotemReadoutObjects.TotemDAQMappingESSourceXML_cfi import *
TotemDAQMappingESSourceXML.mappingFileNames.append("CondFormats/TotemReadoutObjects/xml/ctpps_210_mapping.xml")
TotemDAQMappingESSourceXML.mappingFileNames.append("CondFormats/TotemReadoutObjects/xml/timing_diamond_215_mapping.xml")

from EventFilter.TotemRawToDigi.totemTriggerRawToDigi_cfi import *
totemTriggerRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

from EventFilter.TotemRawToDigi.totemRPRawToDigi_cfi import *
totemRPRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")

from EventFilter.TotemRawToDigi.totemDiamondRPRawToDigi_cfi import *
totemDiamondRPRawToDigi.rawDataTag = cms.InputTag("rawDataCollector")
