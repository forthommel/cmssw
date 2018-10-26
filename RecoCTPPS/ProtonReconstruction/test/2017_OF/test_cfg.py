import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSTestProtonReconstruction", eras.ctpps_2016)

process.load("test_base_cff")
from test_base_cff import *

#process.source.fileNames = cms.untracked.vstring("/store/data/Run2017E/DoubleEG/MINIAOD/17Nov2017-v1/40000/6A3BC091-AFD3-E711-BF8B-0CC47A57CC8A.root")
process.source.fileNames = cms.untracked.vstring("/store/data/Run2017E/DoubleEG/AOD/17Nov2017-v1/40000/AE8D481F-8BD3-E711-B749-02163E019CFF.root")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100000)
)

UseCrossingAngle140(process.source)
