import FWCore.ParameterSet.Config as cms

from EventFilter.TotemRawToDigi.totemVFATRawToDigi_cfi import totemVFATRawToDigi

totemRPRawToDigi = totemVFATRawToDigi.clone(
    subSystem = cms.string('RP'),
)
