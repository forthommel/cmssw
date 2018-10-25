import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "fill_6370/xangle_140";

string stream = "DoubleEG";

string alignments[], a_labels[];
alignments.push("2018_10_25.5"); a_labels.push("2018-10-25.5");

string cols[], c_labels[], c_si_rps[];
cols.push("arm0"); c_labels.push("sector 45 (L)"); c_si_rps.push("rp23");
cols.push("arm1"); c_labels.push("sector 56 (R)"); c_si_rps.push("rp123");

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
		NewPad("fill", "mean of $\xi_{\rm multi} - \xi_{\rm single}$");

		string f = topDir + dataset + "/" + stream + "/alignment_" + alignments[ai] + "/do_fits.root";
		string on = "singleMultiCorrelationPlots/si_" + c_si_rps[ci]  + "_mu_" + cols[ci] + "/p_xi_diff_si_mu_vs_xi_mu";
		
		RootObject hist = RootGetObject(f, on, error=false);
		RootObject fit = RootGetObject(f, on + "|ff", error=false);

		if (!hist.valid)
			continue;
		
		draw(hist, "d0,vl", blue);
		draw(fit, "def", red+1pt);

		limits((0., -0.1), (0.2, +0.1), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=0mm);
