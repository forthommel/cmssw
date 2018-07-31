import root;
import pad_layout;

string topDir = "../../data_eos/";

//string dataset = "phys_no_margin/fill_5027";
string dataset = "phys_no_margin/fill_5275";

string stream = "DoubleEG";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_24.3"); a_labels.push("2018-07-24.3");
alignments.push("2018_07_30.5"); a_labels.push("2018-07-30.5");

string cols[], c_labels[];
cols.push("si_rp2_mu_arm0"); c_labels.push("multi(L) vs.~single(RP2)");
cols.push("si_rp3_mu_arm0"); c_labels.push("multi(L) vs.~single(RP3)");
cols.push("si_rp102_mu_arm1"); c_labels.push("multi(R) vs.~single(RP102)");
cols.push("si_rp103_mu_arm1"); c_labels.push("multi(R) vs.~single(RP103)");

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
		
		draw(obj);

		draw((0, 0)--(0.2, 0.2), black+1pt);

		limits((0.02, 0.02), (0.16, 0.16), Crop);
	}
}

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
