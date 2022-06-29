import FWCore.ParameterSet.Config as cms

from SimTotem.TotemT2Producer.totemT2DigiParameters_cff import totem_t2_digi_parameters

totemT2Digitizer = cms.PSet(
    totem_t2_digi_parameters,
    accumulatorType = cms.string('TotemT2DigiProducer')
)
