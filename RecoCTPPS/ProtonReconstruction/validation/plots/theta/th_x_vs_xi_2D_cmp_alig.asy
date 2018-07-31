import root;
import pad_layout;

string topDir = "../../data_eos/";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_24.3"); a_labels.push("2018-07-24.3");
alignments.push("2018_07_30.5"); a_labels.push("2018-07-30.5");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string dataset = "phys_no_margin/fill_5266";

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
		NewPad("$\xi_{\rm multi}$", "$\th^*_x\ung{\mu rad}$");

		string f = topDir + dataset + "/" + stream + "/alignment_" + alignments[ai] + "/output.root";
		string on = "multiRPPlots/" + cols[ci] + "/h2_th_x_vs_xi";

		RootObject obj = RootGetObject(f, on);
		obj.vExec("Rebin2D", 2, 2);

		draw(scale(1., 1e6), obj);

		limits((0.00, -300), (0.16, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
