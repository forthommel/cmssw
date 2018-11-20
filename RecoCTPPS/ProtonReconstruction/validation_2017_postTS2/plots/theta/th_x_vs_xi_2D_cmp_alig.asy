import root;
import pad_layout;

string topDir = "../../data_eos/";

string alignments[], a_labels[];
alignments.push("2018_10_25.5"); a_labels.push("2018-10-25.5");

string cols[], c_labels[];
real c_xi0s[], c_as[];
cols.push("arm0"); c_labels.push("sector 45 (L)"); c_xi0s.push(0.153); c_as.push(-230);
cols.push("arm1"); c_labels.push("sector 56 (R)"); c_xi0s.push(0.19); c_as.push(-330);

string dataset = "fill_6370/xangle_140";

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

		real y_min = -300e-6, x_cut_min = c_xi0s[ci] + c_as[ci] * y_min;
		real y_max = +300e-6, x_cut_max = c_xi0s[ci] + c_as[ci] * y_max;

		draw((x_cut_min, y_min*1e6)--(x_cut_max, y_max*1e6), magenta+2pt);

		limits((0.00, -300), (0.25, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
