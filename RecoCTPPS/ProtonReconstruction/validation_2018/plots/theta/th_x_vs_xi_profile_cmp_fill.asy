import root;
import pad_layout;

string topDir = "../../data_eos/";

string stream = "ZeroBias";

string alignment = "2018_11_02.3";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string datasets[] = {
	"fill_7334/xangle_140_beta_0.30",
};

TH2_palette = Gradient(blue, heavygreen, yellow, red);

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
		NewPad("$\xi_{\rm multi}$", "mean of $\th^*_x\ung{\mu rad}$");

		string f = topDir + datasets[dsi] + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
		string on = "multiRPPlots/" + cols[ci] + "/p_th_x_vs_xi";

		RootObject hist = RootGetObject(f, on);
		RootObject fit = RootGetObject(f, on + "|ff");

		draw(scale(1., 1e6), hist, "d0,eb", blue);
		draw(scale(1., 1e6), fit, "def", red+1pt);

		limits((0.00, -300), (0.20, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
