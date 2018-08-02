import root;
import pad_layout;

string topDir = "../";

string f = "fast_simu_validation/output_y_vs_x_profiles.root";

string f_opt = "optics_parametrisation_validation/optical_functions.root";

string rp_dirs[], rp_labels[], rp_opt_dirs[];
rp_dirs.push("RP 3"); rp_labels.push("45-210-fr-hr"); rp_opt_dirs.push("ctppsPlotOpticalFunctions_45/ip5_to_station_150_h_1_lhcb2");
rp_dirs.push("RP 23"); rp_labels.push("45-220-fr-hr"); rp_opt_dirs.push("ctppsPlotOpticalFunctions_45/ip5_to_station_150_h_2_lhcb2");
rp_dirs.push("RP 103"); rp_labels.push("56-210-fr-hr"); rp_opt_dirs.push("ctppsPlotOpticalFunctions_56/ip5_to_station_150_h_1_lhcb1");
rp_dirs.push("RP 123"); rp_labels.push("56-220-fr-hr"); rp_opt_dirs.push("ctppsPlotOpticalFunctions_56/ip5_to_station_150_h_2_lhcb1");

//----------------------------------------------------------------------------------------------------

for (int rpi : rp_dirs.keys)
{
	NewPad("$x\ung{mm}$", "$y\ung{mm}$");
	scale(Linear, Linear, Log);

	draw(RootGetObject(topDir + f, rp_dirs[rpi] + "/p_y_vs_x"), "d0,eb", blue);

	draw(scale(1e3, 1e3), RootGetObject(topDir + f_opt, rp_opt_dirs[rpi] + "/g_y0_vs_x0"), "def", red);

	if (find(rp_labels[rpi], "56") >= 0)
		limits((0, 0.5), (15, +0.8), Crop);
	else
		limits((0, -1.4), (15, -0.9), Crop);
	AttachLegend(rp_labels[rpi]);
}

GShipout(hSkip=1mm);
