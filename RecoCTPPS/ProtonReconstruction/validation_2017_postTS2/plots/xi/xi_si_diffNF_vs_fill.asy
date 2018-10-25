import root;
import pad_layout;

include "../fills_samples.asy";
InitDataSets();

string topDir = "../../data_eos/";

string stream = "DoubleEG";
int xangle = 140;

string alignments[], a_labels[];
//alignments.push("2018_10_23.1"); a_labels.push("2018-10-23.1");
//alignments.push("2018_10_25.1"); a_labels.push("2018-10-25.1");
alignments.push("2018_10_25.5"); a_labels.push("2018-10-25.5");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

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

xSizeDef = 30cm;

//yTicksDef = RightTicks(50., 10.);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{xangle: " + format("%u", xangle) + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int ai : alignments.keys)
{
	NewRow();

	NewPadLabel(a_labels[ai]);

	for (int ci : cols.keys)
	{
		NewPad("fill", "mean of $\De^{F-N} \xi_{\rm single}$");

		for (int fi : fill_data.keys)
		{
			for (int dsi : fill_data[fi].datasets.keys)
			{
				if (fill_data[fi].datasets[dsi].xangle != xangle)
					continue;

				string f = topDir + fill_data[fi].datasets[dsi].tag + "/" + stream + "/alignment_" + alignments[ai] + "/do_fits.root";
				string on = "armCorrelationPlots/" + cols[ci] + "/p_xi_si_diffNF_vs_xi_mu|ff";
	
				RootObject obj = RootGetObject(f, on, error=false);
				if (!obj.valid)
					continue;
	
				real d = obj.rExec("GetParameter", 0);
				real d_unc = obj.rExec("GetParError", 0);

				mark m = GetDatasetMark(fill_data[fi].datasets[dsi]);
				pen p = GetDatasetPen(fill_data[fi].datasets[dsi]);

				real x = fi;
				draw((x, d), m+p);
				draw((x, d-d_unc)--(x, d+d_unc), p);
			}
		}

		limits((-1, -2.5e-3), (fill_data.length, +2.5e-3), Crop);
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
