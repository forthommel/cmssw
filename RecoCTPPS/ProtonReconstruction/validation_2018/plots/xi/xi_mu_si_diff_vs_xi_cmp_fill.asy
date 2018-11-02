import root;
import pad_layout;

string topDir = "../../data_eos/";

string datasets[] = {
	"fill_7333/xangle_140_beta_0.30",
};

string stream = "ZeroBias";

string alignment = "2018_11_02.3";

string cols[], c_labels[], c_si_rps[];
cols.push("arm0"); c_labels.push("sector 45 (L)"); c_si_rps.push("rp23");
cols.push("arm1"); c_labels.push("sector 56 (R)"); c_si_rps.push("rp123");

//----------------------------------------------------------------------------------------------------

NewPad(false);
AddToLegend("stream = " + stream);
AddToLegend("alignment = " + replace(alignment, "_", "\_"));
AttachLegend();

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int dsi : datasets.keys)
{
	NewRow();

	NewPadLabel(replace(datasets[dsi], "_", "\_"));

	for (int ci : cols.keys)
	{
		NewPad("$\xi_{\rm multi}$", "mean of $\xi_{\rm single} - \xi_{\rm multi}$");

		string f = topDir + datasets[dsi] + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
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
