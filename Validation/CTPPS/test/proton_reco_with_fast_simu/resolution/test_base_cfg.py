from base_cff import *

# distribution plotter
process.ctppsTrackDistributionPlotter = cms.EDAnalyzer("CTPPSTrackDistributionPlotter",
  tracksTag = cms.InputTag("ctppsLocalTrackLiteProducer"),
  outputFile = cms.string("output_test_base.root")
)

#process.ctppsProtonReconstructionValidator.outputFile = "output_test_base.root"

# processing path
process.p = cms.Path(
    process.generator
    * process.beamDivergenceVtxGenerator
    * process.ctppsFastProtonSimulation

    * process.totemRPUVPatternFinder
    * process.totemRPLocalTrackFitter
    * process.ctppsLocalTrackLiteProducer
    * process.ctppsProtonReconstruction

    #* process.ctppsProtonReconstructionValidator
    * process.ctppsTrackDistributionPlotter
)
