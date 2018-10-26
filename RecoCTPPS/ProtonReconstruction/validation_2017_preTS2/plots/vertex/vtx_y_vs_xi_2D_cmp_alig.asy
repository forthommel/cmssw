import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "phys_no_margin/fill_5288";

string stream = "DoubleEG";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_24.3"); a_labels.push("2018-07-24.3");
alignments.push("2018_07_30.5"); a_labels.push("2018-07-30.5");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

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
		NewPad("$\xi_{\rm multi}$", "$y^*\ung{\mu m}$");

		string f = topDir + dataset + "/" + stream + "/alignment_" + alignments[ai] + "/output.root";
		string on = "multiRPPlots/" + cols[ci] + "/h2_vtx_y_vs_xi";
		RootObject hist = RootGetObject(f, on, error=false);

		if (!hist.valid)
			continue;

		//hist.vExec("Rebin2D", 2, 2);

		//string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
		//string on = "multiRPPlots/" + cols[ci] + "/p_vtx_y_vs_xi|ff_pol1";
		//RootObject fit = RootGetObject(f, on);

		draw(scale(1., 1e3), hist);
		//draw(scale(1., 1e3), fit, black+2pt);

		limits((0.00, -500), (0.16, +500), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
