import root;
import pad_layout;

string topDir = "../../data_eos/";

include "../fills_samples.asy";
//InitDataSets("short");
AddDataSet("phys_margin/fill_4947");
AddDataSet("phys_no_margin/fill_5261");

string stream = "DoubleEG";

//string alignment = "2018_07_17";
//string alignment = "2018_07_24.3";
string alignment = "2018_07_30.5";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{alignment: " + replace(alignment, "_", "\_") + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int fi : fill_data.keys)
{
	for (int dsi : fill_data[fi].datasets.keys)
	{
		NewRow();
		
		string dataset = fill_data[fi].datasets[dsi].tag;
	
		NewPadLabel(replace(dataset, "_", " "));
	
		for (int ci : cols.keys)
		{
			NewPad("$\xi$", "$|t|\ung{GeV^2}$", axesAbove = true);
			scale(Linear, Linear, Log);

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/output.root";
			string on = "multiRPPlots/" + cols[ci] + "/h2_t_vs_xi";
			RootObject hist = RootGetObject(f, on);

			hist.vExec("Rebin2D", 2, 1);

			draw(hist);

			limits((3, 0), (0.20, 4), Crop);
		}
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
