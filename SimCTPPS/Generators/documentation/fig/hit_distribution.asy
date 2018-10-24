import root;
import pad_layout;

string topDir = "../../";

string files[] = {
	"output_shape_smear.root",
};

string rp_dirs[], rp_labels[];
rp_dirs.push("RP 3"); rp_labels.push("45-210-fr-hr");
rp_dirs.push("RP 23"); rp_labels.push("45-220-fr-hr");
rp_dirs.push("RP 103"); rp_labels.push("56-210-fr-hr");
rp_dirs.push("RP 123"); rp_labels.push("56-220-fr-hr");

xTicksDef = LeftTicks(5., 1.);

//----------------------------------------------------------------------------------------------------

for (int fi : files.keys)
{
	NewRow();

	string f = files[fi];

	//NewPadLabel(replace(f, "_", "\_"));

	for (int rpi : rp_dirs.keys)
	{
		if (rpi == 2)
			NewRow();

		NewPad("$x\ung{mm}$", "$y\ung{mm}$");
		scale(Linear, Linear, Log);

		RootObject hist = RootGetObject(topDir + f, rp_dirs[rpi] + "/h2_y_vs_x");
		hist.vExec("Rebin2D", 2, 2);

		draw(hist);
		limits((0, -10), (20, 10), Crop);
		AttachLegend(rp_labels[rpi]);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
