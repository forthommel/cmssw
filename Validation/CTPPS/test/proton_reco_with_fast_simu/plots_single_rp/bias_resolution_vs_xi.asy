import root;
import pad_layout;

string topDir = "../resolution/1E4/";

TGraph_errorBar = None;

string cols[];
string c_tags[];
cols.push("sector 45 (220-fr)"); c_tags.push("3");
cols.push("sector 56 (220-fr)"); c_tags.push("103");

string rows[];
string r_tags[], r_labs[];
rows.push("\hbox{bias}"); r_tags.push("p_de_"); r_labs.push("mean");
rows.push("\hbox{resolution}"); r_tags.push("g_rms_de_"); r_labs.push("RMS");

string thetaSize = "Large";

string levels[];
string l_tags[];
pen l_pens[];
levels.push("$y^*$"); l_tags.push("level_1"); l_pens.push(black);
levels.push("$y^*$, $x^*$"); l_tags.push("level_2"); l_pens.push(red);
levels.push("$y^*$, $x^*$, beam. div."); l_tags.push("level_3"); l_pens.push(blue);
levels.push("$y^*$, $x^*$, beam. div., det.~resol."); l_tags.push("level_4"); l_pens.push(heavygreen);

string quantities[];
string q_labels[];
string q_units[];
real q_scales[], q_mean_lims[], q_RMS_lims[];
quantities.push("xi"); q_labels.push("\xi"); q_units.push(""); q_scales.push(1.); q_mean_lims.push(0.01); q_RMS_lims.push(0.04);
quantities.push("th_x"); q_labels.push("\th^*_x"); q_units.push("\ung{\mu rad}"); q_scales.push(1e6); q_mean_lims.push(15.); q_RMS_lims.push(30.);
quantities.push("th_y"); q_labels.push("\th^*_y"); q_units.push("\ung{\mu rad}"); q_scales.push(1e6); q_mean_lims.push(15.); q_RMS_lims.push(30.);
quantities.push("vtx_y"); q_labels.push("y^*"); q_units.push("\ung{\mu m}"); q_scales.push(1e3); q_mean_lims.push(100.); q_RMS_lims.push(200.);
quantities.push("t"); q_labels.push("|t|"); q_units.push("\ung{GeV^2}"); q_scales.push(1.); q_mean_lims.push(0.5); q_RMS_lims.push(1.);

xTicksDef = LeftTicks(0.05, 0.01);

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
			NewPad("$\xi(\rm simu)$",  r_labs[ri] + " of $"+ql+"({\rm reco}) - "+ql+"({\rm simu}) " + q_units[qi] + "$");
			scale(Linear, Linear(true));
	
			for (int li : levels.keys)
			{
				string f = topDir + "th_" + thetaSize + "_" + l_tags[li] + "_validation.root";
				string o = "single rp/" + c_tags[ci] + "/" + r_tags[ri] + quantities[qi]+"_vs_xi_simu";
	
				pen p = l_pens[li];

				if (r_labs[ri] == "mean")
					draw(scale(1., q_scales[qi]), RootGetObject(f, o), "d0,eb", p);
				if (r_labs[ri] == "RMS")
					draw(scale(1., q_scales[qi]), RootGetObject(f, o), "d0,l,p", p, mCi+2pt+p);
			}
	
			if (r_labs[ri] == "mean")
				limits((0.01, -q_mean_lims[qi]), (0.21, +q_mean_lims[qi]), Crop);
			if (r_labs[ri] == "RMS")
				limits((0.01, 0.), (0.21, q_RMS_lims[qi]), Crop);
		}
	
		if (ri == 0)
		{
			NewPad(false);
	
			for (int li : levels.keys)
			{
				AddToLegend(levels[li], l_pens[li]);
			}
	
			AttachLegend();
		}
	}

	GShipout("bias_resolution_vs_xi_" + quantities[qi], vSkip=0mm);
}
