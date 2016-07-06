import FWCore.ParameterSet.Config as cms

from EventFilter.TotemRawToDigi.totemVFATRawToDigi_cfi import totemVFATRawToDigi

totemDiamondRPRawToDigi = totemVFATRawToDigi.clone(
    subSystem = cms.string("DiamondRP"),
)
