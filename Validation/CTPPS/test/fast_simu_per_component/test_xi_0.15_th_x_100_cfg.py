from base_cff import *

process.generator.xi_min = process.generator.xi_max = 0.15

process.generator.theta_x_mean = 100e-6  # in rad

process.ctppsTrackDistributionPlotter.outputFile = "output_xi_0.15_th_x_100.root"
