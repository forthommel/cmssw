from base_cff import *

process.generator.xi_min = process.generator.xi_max = 0.15

process.beamDivergenceVtxGenerator.vertexMeanX = 10e-4 # in cm

process.ctppsTrackDistributionPlotter.outputFile = "output_xi_0.15_vtx_x_10.root"
