import FWCore.ParameterSet.Config as cms

totem_t2_digi_parameters = cms.PSet(
    hitsProducer = cms.InputTag('g4SimHits:TotemHitsT2Scint'),
    algo = cms.string('trivial')
)
