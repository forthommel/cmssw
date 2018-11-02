import root;
import pad_layout;

string topDir = "../../data_eos/";

include "../fills_samples.asy";
InitDataSets("short");

string stream = "ZeroBias";

string alignment = "2018_11_02.3";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
AddToLegend("stream = " + stream);
AddToLegend("alignment = " + replace(alignment, "_", "\_"));
AttachLegend();

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
			NewPad("$\xi_{\rm multi}$", "$\th^*_x\ung{\mu rad}$");

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/output.root";
			string on = "multiRPPlots/" + cols[ci] + "/h2_th_x_vs_xi";
			RootObject hist = RootGetObject(f, on);
			//hist.vExec("Rebin2D", 2, 2);

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
			string on = "multiRPPlots/" + cols[ci] + "/p_th_x_vs_xi|ff_pol1";
			RootObject fit = RootGetObject(f, on);

			draw(scale(1., 1e6), hist);
			//draw(scale(1., 1e6), fit, black+2pt);

			limits((0.00, -300), (0.16, +300), Crop);
		}
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
