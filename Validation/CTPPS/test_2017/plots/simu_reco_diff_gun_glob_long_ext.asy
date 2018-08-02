import root;
import pad_layout;

string topDir = "../";

string f_without = "fast_simu_validation/output_long_extrapolation.root";

string rp_dirs[], rp_labels[];
rp_dirs.push("RP 3"); rp_labels.push("45-210-fr-hr");
rp_dirs.push("RP 23"); rp_labels.push("45-220-fr-hr");
rp_dirs.push("RP 103"); rp_labels.push("56-210-fr-hr");
rp_dirs.push("RP 123"); rp_labels.push("56-220-fr-hr");

ySizeDef = 4cm;

xTicksDef = LeftTicks(20., 10.);

//----------------------------------------------------------------------------------------------------

for (int rpi : rp_dirs.keys)
{
	NewPad("$x_{\rm reco} - x_{\rm simu}\ung{\mu m}$");
	scale(Linear, Linear, Log);

	draw(scale(1e3, 1e3), RootGetObject(topDir + f_without, rp_dirs[rpi] + "/h_de_x"), "vl", blue);
	//draw(scale(1e3, 1e3), RootGetObject(topDir + f_with, rp_dirs[rpi] + "/h_de_x"), "vl", red);

	xlimits(-100, +100, Crop);

	AttachLegend(rp_labels[rpi]);
}

//----------------------------------------------------------------------------------------------------
NewRow();

for (int rpi : rp_dirs.keys)
{
	NewPad("$y_{\rm reco} - y_{\rm simu}\ung{\mu m}$");
	scale(Linear, Linear, Log);

	draw(scale(1e3, 1e3), RootGetObject(topDir + f_without, rp_dirs[rpi] + "/h_de_y"), "vl", blue);
	//draw(scale(1e3, 1e3), RootGetObject(topDir + f_with, rp_dirs[rpi] + "/h_de_y"), "vl", red);

	xlimits(-100, +100, Crop);

	AttachLegend(rp_labels[rpi]);
}

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
