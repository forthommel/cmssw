import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "fill_7334/xangle_140_beta_0.30";

string stream = "ZeroBias";

string alignments[], a_labels[];
//alignments.push("PRELIMINARY"); a_labels.push("PRELIMINARY");
//alignments.push("2018_11_01.1"); a_labels.push("2018-11-01.1");
//alignments.push("2018_11_02.1"); a_labels.push("2018-11-02.1");
alignments.push("2018_11_02.3"); a_labels.push("2018-11-02.3");

string cols[], c_labels[];
cols.push("si_rp3_mu_arm0"); c_labels.push("multi(L) vs.~single(RP3)");
cols.push("si_rp23_mu_arm0"); c_labels.push("multi(L) vs.~single(RP23)");
cols.push("si_rp103_mu_arm1"); c_labels.push("multi(R) vs.~single(RP103)");
cols.push("si_rp123_mu_arm1"); c_labels.push("multi(R) vs.~single(RP123)");

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
AddToLegend("dataset = " + replace(dataset, "_", "\_"));
AddToLegend("stream = " + stream);
AttachLegend();

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
