import root;
import pad_layout;
include "../fills_samples.asy";

string topDir = "../../data_eos/";
InitDataSets();

string stream = "DoubleEG";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_24.3"); a_labels.push("2018-07-24.3");
alignments.push("2018_07_30.5"); a_labels.push("2018-07-30.5");

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

xSizeDef = 15cm;

yTicksDef = RightTicks(50., 10.);

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
		NewPad("fill", "mean of $y^*\ung{\mu m}$");

		for (int fi : fill_data.keys)
		{
			for (int dsi : fill_data[fi].datasets.keys)
			{
				string f = topDir + fill_data[fi].datasets[dsi].tag + "/" + stream + "/alignment_" + alignments[ai] + "/do_fits.root";
				string on = "multiRPPlots/" + cols[ci] + "/p_vtx_y_vs_xi|ff";
	
				RootObject obj = RootGetObject(f, on, error=false);
				if (!obj.valid)
					continue;
	
				real d = obj.rExec("GetParameter", 0) * 1e3;
				real d_unc = obj.rExec("GetParError", 0) * 1e3;

				mark m = GetDatasetMark(fill_data[fi].datasets[dsi]);
				pen p = GetDatasetPen(fill_data[fi].datasets[dsi]);

				real x = fi;
				draw((x, d), m+p);
				draw((x, d-d_unc)--(x, d+d_unc), p);
			}
		}

		limits((-1, -100.), (fill_data.length, +100), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
