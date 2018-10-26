import root;
import pad_layout;

string topDir = "../../data_eos/";

string alignments[], a_labels[];
alignments.push("2018_10_26.1"); a_labels.push("2018-10-26.1");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string dataset = "fill_6041/xangle_140";

string stream = "DoubleEG";

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{dataset: " + replace(dataset, "_", "\_") + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int ai : alignments.keys)
{
	NewRow();

	NewPadLabel(a_labels[ai]);

	for (int ci : cols.keys)
	{
		NewPad("$\xi_{\rm multi}$", "$\th^*_y\ung{\mu rad}$");

		string f = topDir + dataset + "/" + stream + "/alignment_" + alignments[ai] + "/output.root";
		string on = "multiRPPlots/" + cols[ci] + "/h2_th_y_vs_xi";

		RootObject obj = RootGetObject(f, on, error = false);

		if (!obj.valid)
			continue;

		obj.vExec("Rebin2D", 2, 2);

		draw(scale(1., 1e6), obj);

		limits((0.00, -300), (0.20, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
