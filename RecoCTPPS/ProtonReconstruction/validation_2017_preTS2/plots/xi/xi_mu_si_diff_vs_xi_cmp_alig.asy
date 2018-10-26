import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "fill_6041/xangle_140";

string stream = "DoubleEG";

string alignments[], a_labels[];
//alignments.push("2018_10_26.1"); a_labels.push("2018-10-26.1");
//alignments.push("2018_10_26.2"); a_labels.push("2018-10-26.2");
alignments.push("2018_10_26.3"); a_labels.push("2018-10-26.3");

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
		NewPad("$\xi_{\rm multi}$", "mean of $\xi_{\rm single} - \xi_{\rm multi}$");

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
