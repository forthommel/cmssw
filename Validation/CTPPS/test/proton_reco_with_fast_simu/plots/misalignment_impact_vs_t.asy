import root;
import pad_layout;

string topDir = "../misalignment/1E4_level_1/";

TGraph_errorBar = None;

string cols[];
string c_tags[];
cols.push("sector 45"); c_tags.push("0");
cols.push("sector 56"); c_tags.push("1");

string rows[];
string r_tags[];
rows.push("\hbox{horizontal}\hbox{misalignment}"); r_tags.push("x");
rows.push("\hbox{vertical}\hbox{misalignment}"); r_tags.push("y");

string level = "level4";

string scenarios[], s_tags[];
pen s_pens[];
scenarios.push("none"); s_tags.push("none"); s_pens.push(black);
scenarios.push("near: +, far: +"); s_tags.push("<proj>_sym"); s_pens.push(red);
scenarios.push("near: -, far: +"); s_tags.push("<proj>_asym"); s_pens.push(blue);

string quantities[];
string q_labels[];
string q_units[];
real q_scales[], q_limits[];
/*
quantities.push("xi"); q_labels.push("\xi"); q_units.push(""); q_scales.push(1.); q_limits.push(0.005);
quantities.push("th_x"); q_labels.push("\th^*_x"); q_units.push("\ung{\mu rad}"); q_scales.push(1e6); q_limits.push(10);
quantities.push("th_y"); q_labels.push("\th^*_y"); q_units.push("\ung{\mu rad}"); q_scales.push(1e6); q_limits.push(15);
quantities.push("vtx_y"); q_labels.push("y^*"); q_units.push("\ung{\mu m}"); q_scales.push(1e3); q_limits.push(200);
*/
quantities.push("t"); q_labels.push("|t|"); q_units.push("\ung{GeV^2}"); q_scales.push(1.); q_limits.push(0.5);

xTicksDef = LeftTicks(1., 0.5);

//----------------------------------------------------------------------------------------------------

for (int qi : quantities.keys)
{
	write("* " + quantities[qi]);

	NewPad();
	for (int ci : cols.keys)
	{
		NewPad(false);
		label("{\SetFontSizesXX " + cols[ci] + "}");
	}
	
	for (int ri : rows.keys)
	{
		NewRow();
	
		NewPad(false);
		label("\vbox{\SetFontSizesXX " + rows[ri] + "}");
	
		for (int ci : cols.keys)
		{
			string ql = q_labels[qi];
			NewPad("$|t|(\rm simu)\ung{GeV^2}$", "mean of $"+ql+"({\rm reco}) - "+ql+"({\rm simu}) " + q_units[qi] + "$");
			scale(Linear, Linear(true));
	
			for (int si : scenarios.keys)
			{
				string s_tag = replace(s_tags[si], "<proj>", r_tags[ri]);

				string f = topDir + s_tag + "_validation.root";

				string o = "multi rp/" + c_tags[ci] + "/p_de_"+quantities[qi]+"_vs_t_simu";
	
				pen p = s_pens[si];
	
				draw(scale(1., q_scales[qi]), RootGetObject(f, o), "d0,eb", p);
			}
	
			limits((0., -q_limits[qi]), (5., +q_limits[qi]), Crop);
		}

		if (ri == 0)
		{
			NewPad(false);
	
			for (int si : scenarios.keys)
			{
				AddToLegend(scenarios[si], s_pens[si]);
			}
	
			AttachLegend();
		}	
	}

	GShipout("misalignment_impact_vs_t_" + quantities[qi], vSkip=0mm);
}
