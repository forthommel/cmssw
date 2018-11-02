import root;
import pad_layout;

string topDir = "../../data_eos/";

string datasets[] = {
	"phys_no_margin/fill_5027",
	"phys_no_margin/fill_5261",
	"phys_no_margin/fill_5265",
};

string stream = "DoubleEG";

string alignment = "2018_11_02.3";

string cols[], c_labels[];
cols.push("si_rp2_mu_arm0"); c_labels.push("multi(L) vs.~single(RP2)");
//cols.push("si_rp3_mu_arm0"); c_labels.push("multi(L) vs.~single(RP3)");
cols.push("si_rp102_mu_arm1"); c_labels.push("multi(R) vs.~single(RP102)");
//cols.push("si_rp103_mu_arm1"); c_labels.push("multi(R) vs.~single(RP103)");

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{alignment: " + replace(alignment, "_", "\_") + "}}");

for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int dsi : datasets.keys)
{
	NewRow();

	NewPadLabel(replace(datasets[dsi], "_", " "));

	for (int ci : cols.keys)
	{
		NewPad("$\th^*_y (\rm single)\ung{\mu rad}$", "$\th^*_y (\rm multi)\ung{\mu rad}$", axesAbove=true);
		scale(Linear, Linear, Log);

		string f = topDir + datasets[dsi] + "/" + stream + "/alignment_" + alignment + "/output.root";
		string on = "singleMultiCorrelationPlots/" + cols[ci] + "/h2_th_y_mu_vs_th_y_si";
		
		RootObject obj = RootGetObject(f, on, error=true);
		if (!obj.valid)
			continue;
		
		draw(scale(1e6, 1e6), obj);

		draw((-200, -200)--(+200, +200), black+1pt);

		limits((-200, -200), (+200, +200), Crop);

		xaxis(YEquals(0, false), black+1pt, above=true);
		yaxis(XEquals(0, false), black+1pt, above=true);
	}
}

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=1mm, vSkip=0mm);
