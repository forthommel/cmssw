import root;
import pad_layout;
include "../fills_samples.asy";

string topDir = "../../data_eos/";
InitDataSets();

string stream = "DoubleEG";
int xangle = 140;

string alignments[], a_labels[];
alignments.push("2018_10_26.4"); a_labels.push("2018-10-26.4");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string projections[];
pen p_pens[];
projections.push("y"); p_pens.push(red);

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

xSizeDef = 35cm;

yTicksDef = RightTicks(10., 5.);

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
		NewPad("fill", "mean of $\th^*_y\ung{\mu rad}$");

		for (int pri : projections.keys)
		{
			for (int fi : fill_data.keys)
			{
				for (int dsi : fill_data[fi].datasets.keys)
				{
					if (fill_data[fi].datasets[dsi].xangle != xangle)
						continue;

					string f = topDir + fill_data[fi].datasets[dsi].tag + "/" + stream + "/alignment_" + alignments[ai] + "/do_fits.root";
					string on = "multiRPPlots/" + cols[ci] + "/p_th_" + projections[pri] + "_vs_xi|ff";
		
					RootObject obj = RootGetObject(f, on, error=false);
					if (!obj.valid)
						continue;
		
					real d = obj.rExec("GetParameter", 0) * 1e6;
					real d_unc = obj.rExec("GetParError", 0) * 1e6;

					mark m = GetDatasetMark(fill_data[fi].datasets[dsi]);
					pen p = p_pens[pri];

					real x = fi;
					draw((x, d), m+p);
					draw((x, d-d_unc)--(x, d+d_unc), p);
				}
			}
		}

		limits((-1, -80.), (fill_data.length, +80.), Crop);

		xaxis(YEquals(0., false), dashed);
	}
}

//----------------------------------------------------------------------------------------------------

NewPad(false);

AddToLegend("$\th^*_x$", blue);
AddToLegend("$\th^*_y$", red);

AttachLegend();

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=0mm, vSkip=0mm);
