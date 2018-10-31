import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("CTPPSTestProtonReconstruction", eras.ctpps_2016)

process.load("test_base_cff")
from test_base_cff import *

process.source.fileNames = cms.untracked.vstring("/store/data/Run2018B/SingleMuon/MINIAOD/17Sep2018-v1/60000/F2A6DA92-8FCA-ED4C-99CC-3C19DEDCD470.root")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100000)
)
