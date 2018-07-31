import root;
import pad_layout;

include "../fills_samples.asy";
InitDataSets();

string topDir = "../../data_eos/";

string stream = "DoubleEG";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_24.3"); a_labels.push("2018-07-24.3");
alignments.push("2018_07_30.5"); a_labels.push("2018-07-30.5");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string xi_si_rps[][];
string xi_si_labels[];
pen xi_si_pens[];
xi_si_rps.push(new string[] { "rp2", "rp3" });
xi_si_rps.push(new string[] { "rp102", "rp103" });
xi_si_labels.push("$\xi_{\rm single}$ from near RP"); xi_si_pens.push(blue);
xi_si_labels.push("$\xi_{\rm single}$ from far RP"); xi_si_pens.push(red);

//----------------------------------------------------------------------------------------------------

string TickLabels(real x)
{
	if (x >=0 && x < fill_data.length)
	{
		int ix = (int) x;
		return format("%u", fill_data[ix].fill);
	} else {
		return "";
	}
}

xTicksDef = LeftTicks(rotate(90)*Label(""), TickLabels, Step=1, step=0);

xSizeDef = 15cm;

//yTicksDef = RightTicks(50., 10.);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int ai : alignments.keys)
{
	NewRow();

	NewPadLabel(a_labels[ai]);

	for (int ci : cols.keys)
	{
		NewPad("fill", "mean of $\xi_{\rm multi} - \xi_{\rm single}$");

		for (int xsi : xi_si_labels.keys)
		{
			for (int fi : fill_data.keys)
			{
				for (int dsi : fill_data[fi].datasets.keys)
				{
					string f = topDir + fill_data[fi].datasets[dsi].tag + "/" + stream + "/alignment_" + alignments[ai] + "/do_fits.root";
					string on = "singleMultiCorrelationPlots/si_" + xi_si_rps[ci][xsi]  + "_mu_" + cols[ci] + "/p_xi_diff_si_mu_vs_xi_mu|ff";
		
					RootObject obj = RootGetObject(f, on, error=false);
					if (!obj.valid)
						continue;
		
					real d = obj.rExec("GetParameter", 0);
					real d_unc = obj.rExec("GetParError", 0);

					mark m = GetDatasetMark(fill_data[fi].datasets[dsi]);
					pen p = xi_si_pens[xsi];

					real x = fi;
					draw((x, d), m+p);
					draw((x, d-d_unc)--(x, d+d_unc), p);
				}
			}
		}

		limits((-1, -0.01), (fill_data.length, +0.01), Crop);
	}
}

//----------------------------------------------------------------------------------------------------

/*
NewPad(false);

AddToLegend("$\th^*_x$", blue);
AddToLegend("$\th^*_y$", red);

AttachLegend();
*/

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
