import root;
import pad_layout;

string topDir = "../";

string rp_tags[], rp_labels[], rp_new_objs[], rp_files[];
rp_tags.push("3"); rp_labels.push("45-210-fr"); rp_new_objs.push("ip5_to_station_150_h_2_lhcb2"); rp_files.push("test_45_extra.root");
//rp_tags.push("2"); rp_labels.push("45-210-nr"); rp_new_objs.push("ip5_to_station_150_h_1_lhcb2"); rp_files.push("test_45.root");
//rp_tags.push("102"); rp_labels.push("56-210-nr"); rp_new_objs.push("ip5_to_station_150_h_1_lhcb1"); rp_files.push("test_56.root");
rp_tags.push("103"); rp_labels.push("56-210-fr"); rp_new_objs.push("ip5_to_station_150_h_2_lhcb1"); rp_files.push("test_56_extra.root");

//----------------------------------------------------------------------------------------------------

void PlotOneSet(string fl, string label, string dirName)
{
	string f = topDir + fl;

	NewPad("$\xi$", "$\De x\ung{\mu m}$");
	draw(scale(1., 1e6), RootGetObject(f, dirName + "/p_de_x_vs_xi"), blue);

	NewPad("$\th^*_x\ung{\mu rad}$", "$\De x\ung{\mu m}$");
	draw(scale(1e6, 1e6), RootGetObject(f, dirName + "/p_de_x_vs_th_x"), blue);

	NewPad("$\th^*_y\ung{\mu rad}$", "$\De x\ung{\mu m}$");
	draw(scale(1e6, 1e6), RootGetObject(f, dirName + "/p_de_x_vs_th_y"), blue);

	NewRow();

	NewPad("$\xi$", "$\De y\ung{\mu m}$");
	draw(scale(1., 1e6), RootGetObject(f, dirName + "/p_de_y_vs_xi"), red);

	NewPad("$\th^*_x\ung{\mu rad}$", "$\De y\ung{\mu m}$");
	draw(scale(1e6, 1e6), RootGetObject(f, dirName + "/p_de_y_vs_th_x"), red);

	NewPad("$\th^*_y\ung{\mu rad}$", "$\De y\ung{\mu m}$");
	draw(scale(1e6, 1e6), RootGetObject(f, dirName + "/p_de_y_vs_th_y"), red);
}

//----------------------------------------------------------------------------------------------------

for (int rpi : rp_tags.keys)
{
	PlotOneSet(rp_files[rpi], rp_labels[rpi], rp_new_objs[rpi]);
	
	GShipout("diff_opt_par_fcn_RP_" + rp_tags[rpi] + "_extra", hSkip=1mm, vSkip=0mm);
}

//----------------------------------------------------------------------------------------------------

