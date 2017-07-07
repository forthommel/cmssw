import FWCore.ParameterSet.Config as cms

protons = cms.EDProducer('ProtonReco',
    tracksTag = cms.InputTag('ctppsOpticsParameterisation'),
    protonRecoAlgo = cms.PSet(
        filename = cms.FileInPath('RecoCTPPS/ProtonReco/data/ctpps_optics_9mar2017.root'),
        interpolationCurves = cms.VPSet(
            cms.PSet(
                potId = cms.uint32(0x76100000), # 2
                splineName = cms.string(''),
            ),
            cms.PSet(
                potId = cms.uint32(0x76180000), # 3
                splineName = cms.string(''),
            ),
            cms.PSet(
                potId = cms.uint32(0x77100000), # 102
                splineName = cms.string(''),
            ),
            cms.PSet(
                potId = cms.uint32(0x77180000), # 103
                splineName = cms.string(''),
            ),
        ),
    ),
    #alignmentFile = cms.FileInPath(''),
)
