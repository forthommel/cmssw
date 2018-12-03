import root;
import pad_layout;

string topDir = "./";


string rp_tags[], rp_labels[], rp_old_objs[], rp_new_objs[];
rp_tags.push("3"); rp_labels.push("45-210-fr");   rp_new_objs.push("ip5_to_station_150_h_2_lhcb2"); rp_old_objs.push("ctppsPlotOpticalFunctions_45/ip5_to_station_150_h_2_lhcb2");
rp_tags.push("2"); rp_labels.push("45-210-nr");   rp_new_objs.push("ip5_to_station_150_h_1_lhcb2"); rp_old_objs.push("ctppsPlotOpticalFunctions_45/ip5_to_station_150_h_1_lhcb2");
rp_tags.push("102"); rp_labels.push("56-210-nr"); rp_new_objs.push("ip5_to_station_150_h_1_lhcb1"); rp_old_objs.push("ctppsPlotOpticalFunctions_56/ip5_to_station_150_h_1_lhcb1");
rp_tags.push("103"); rp_labels.push("56-210-fr"); rp_new_objs.push("ip5_to_station_150_h_2_lhcb1"); rp_old_objs.push("ctppsPlotOpticalFunctions_56/ip5_to_station_150_h_2_lhcb1");

TGraph_x_max = 0.151;

//----------------------------------------------------------------------------------------------------

void PlotAll(int rpi, string objName, real y_scale=1)
{
	scale(Linear, Linear(true));

	{
		RootObject obj = RootGetObject("test_45.root", rp_new_objs[rpi]+"/p_" + objName, error = false);
		if (obj.valid)
			draw(scale(1, y_scale), obj, blue);
	}

	{
		RootObject obj = RootGetObject("test_56.root", rp_new_objs[rpi]+"/p_" + objName, error = false);
		if (obj.valid)
			draw(scale(1, y_scale), obj, blue);
	}

	{
		RootObject obj = RootGetObject("../optics_parametrisation_validation/optical_functions.root", rp_old_objs[rpi]+"/g_" + objName, error = false);
		if (obj.valid)
			draw(scale(1, y_scale), obj, red);
	}
}

//----------------------------------------------------------------------------------------------------

for (int rpi : rp_tags.keys)
{
	NewRow();

	NewPad(false);
	label("{\SetFontSizesXX " + rp_labels[rpi] + "}");

	//--------------------

	NewPad("$\xi$", "$x_D\ung{mm}$");
	PlotAll(rpi, "x0_vs_xi", 1e3);

	//--------------------

	NewPad("$\xi$", "$D_x\ung{mm}$");
	//currentpad.yTicks = RightTicks(50., 10.);
	PlotAll(rpi, "D_x_vs_xi", 1e3);

	//draw((0., 70.)--(0.05, 74.)--(0.1, 77.)--(0.15, 80.), magenta, mCi+2pt+magenta);

	//--------------------

	NewPad("$\xi$", "$v_x$");
	PlotAll(rpi, "v_x_vs_xi", 1);

	//--------------------

	NewPad("$\xi$", "$L_x\ung{m}$");
	PlotAll(rpi, "L_x_vs_xi", 1);

	//--------------------

	NewPad("$\xi$", "$E_{14}\ung{m}$");
	PlotAll(rpi, "E_14_vs_xi", 1);

	//--------------------
	//--------------------

	NewPad("$\xi$", "$y_D\ung{mm}$");
	PlotAll(rpi, "y0_vs_xi", 1e3);

	//--------------------

	NewPad("$\xi$", "$D_y\ung{mm}$");
	PlotAll(rpi, "D_y_vs_xi", 1e3);

	//--------------------

	NewPad("$\xi$", "$v_y$");
	PlotAll(rpi, "v_y_vs_xi", 1);

	//--------------------

	NewPad("$\xi$", "$L_y\ung{m}$");
	PlotAll(rpi, "L_y_vs_xi", 1);

	//--------------------

	NewPad("$\xi$", "$E_{32}\ung{m}$");
	PlotAll(rpi, "E_32_vs_xi", 1);
}

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
