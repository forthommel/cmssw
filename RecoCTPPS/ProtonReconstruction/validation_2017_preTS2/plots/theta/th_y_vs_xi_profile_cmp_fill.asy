import root;
import pad_layout;

string topDir = "../../data_eos/";

string alignment = "2018_10_26.4";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

string datasets[] = {
	"fill_5976/xangle_140",
	"fill_6156/xangle_140",
};

string stream = "DoubleEG";

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{alignment: " + replace(alignment, "_", "\_") + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int dsi : datasets.keys)
{
	NewRow();

	NewPadLabel(replace(datasets[dsi], "_", "\_"));

	for (int ci : cols.keys)
	{
		NewPad("$\xi_{\rm multi}$", "mean of $\th^*_y\ung{\mu rad}$");

		string f = topDir + datasets[dsi] + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
		string on = "multiRPPlots/" + cols[ci] + "/p_th_y_vs_xi";

		RootObject hist = RootGetObject(f, on);
		RootObject fit = RootGetObject(f, on + "|ff");

		draw(scale(1., 1e6), hist, "d0,eb", blue);
		draw(scale(1., 1e6), fit, "def", red+1pt);

		limits((0.00, -300), (0.20, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
