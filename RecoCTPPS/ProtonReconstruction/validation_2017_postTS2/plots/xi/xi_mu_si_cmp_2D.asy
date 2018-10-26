import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "fill_6370/xangle_140";

string stream = "DoubleEG";

string alignments[], a_labels[];
//alignments.push("2018_10_23.1"); a_labels.push("2018-10-23.1");
alignments.push("2018_10_25.5"); a_labels.push("2018-10-25.5");

string cols[], c_labels[];
cols.push("si_rp3_mu_arm0"); c_labels.push("multi(L) vs.~single(RP3)");
cols.push("si_rp23_mu_arm0"); c_labels.push("multi(L) vs.~single(RP23)");
cols.push("si_rp103_mu_arm1"); c_labels.push("multi(R) vs.~single(RP103)");
cols.push("si_rp123_mu_arm1"); c_labels.push("multi(R) vs.~single(RP123)");

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
		NewPad("$\xi_{\rm single}$", "$\xi_{\rm multi}$");

		string f = topDir + dataset + "/" + stream + "/alignment_" + alignments[ai] + "/output.root";
		string on = "singleMultiCorrelationPlots/" + cols[ci] + "/h2_xi_mu_vs_xi_si";
		
		RootObject obj = RootGetObject(f, on, error=true);
		if (!obj.valid)
			continue;

		obj.vExec("Rebin2D", 2, 2);
		
		draw(obj);

		draw((0, 0)--(0.25, 0.25), black+1pt);

		limits((0.0, 0.0), (0.25, 0.25), Crop);
	}
}

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
