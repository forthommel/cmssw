import root;
import pad_layout;

string topDir = "../../data_eos/";

include "../fills_samples.asy";
//InitDataSets("short");
AddDataSet("phys_margin/fill_4947");
AddDataSet("phys_no_margin/fill_5038");
AddDataSet("phys_no_margin/fill_5261");
AddDataSet("phys_no_margin/fill_5288");

string stream = "DoubleEG";

//string alignment = "2018_07_17";
//string alignment = "2018_07_24.3";
string alignment = "2018_07_30.5";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string rows[], r_labels[];
rows.push("range1"); r_labels.push("$0.04 < \xi < 0.07$");
rows.push("range2"); r_labels.push("$0.07 < \xi < 0.10$");
rows.push("range3"); r_labels.push("$0.10 < \xi < 0.13$");

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

real ExpDist(real mt)
{
	real B = 3.0;
	return B * exp(-B * mt);
}

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{alignment: " + replace(alignment, "_", "\_") + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int ri : rows.keys)
{
	NewRow();

	NewPadLabel(r_labels[ri]);
	
	for (int ci : cols.keys)
	{
		NewPad("$|t|\ung{GeV^2}$");
		scale(Linear, Log);

		for (int fi : fill_data.keys)
		{
			for (int dsi : fill_data[fi].datasets.keys)
			{
				string dataset = fill_data[fi].datasets[dsi].tag;

				string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/output.root";
				string on = "multiRPPlots/" + cols[ci] + "/h_t_xi_" + rows[ri];
				RootObject hist = RootGetObject(f, on);

				pen p = StdPen(fi);

				draw(hist, "N,eb", p);
			}
		}

		//draw(graph(ExpDist, 0., 4.), black+dashed);

		limits((0, 1e-2), (4, 5), Crop);
	}
}

NewPad(false);

for (int fi : fill_data.keys)
	AddToLegend(format("fill %u", fill_data[fi].fill), StdPen(fi));

AttachLegend();

GShipout(hSkip=0mm, vSkip=0mm);
