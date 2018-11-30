import root;
import pad_layout;

string f = "../../ppxzGeneratorValidation.root";

string particles[];
particles.push("X"); 
particles.push("Z"); 

xSizeDef = 7cm;

//----------------------------------------------------------------------------------------------------

void MakeOnePlot(string tag, string alabel, string caption="")
{
	NewPad(alabel);
	draw(RootGetObject(f, "before simulation/" + tag), "vl,eb", blue);
	draw(RootGetObject(f, "after simulation/" + tag), "vl,eb", red);

	if (caption != "")
		AttachLegend(caption);
}

//----------------------------------------------------------------------------------------------------

string quantities[], q_labels[];
quantities.push("p_T"); q_labels.push("$p_{\rm T}\ung{GeV}$");
quantities.push("p_z"); q_labels.push("$p_z\ung{GeV}$");
quantities.push("p_tot"); q_labels.push("$p\ung{GeV}$");
quantities.push("theta"); q_labels.push("$\th$");
quantities.push("eta"); q_labels.push("$\et$");

string particles[], p_labels[];
particles.push("X"); p_labels.push("X");
particles.push("X_pr1"); p_labels.push("X, product 1");
particles.push("X_pr2"); p_labels.push("X, product 2");
particles.push("Z"); p_labels.push("Z");
particles.push("l_mi"); p_labels.push("$\rm l^-$");
particles.push("l_pl"); p_labels.push("$\rm l^+$");

for (int qi : quantities.keys)
{
	for (int pti : particles.keys)
	{
		if (pti == 3)
			NewRow();

		MakeOnePlot("h_" + quantities[qi] + "_" + particles[pti], q_labels[qi], p_labels[pti]);
	}

	GShipout(quantities[qi], hSkip=2mm, vSkip=0mm);
}

//----------------------------------------------------------------------------------------------------

MakeOnePlot("h_m_Z", "$m_{\rm Z}\ung{GeV}$");

MakeOnePlot("h_m_XZ", "$m_{\rm XZ}\ung{GeV}$");

MakeOnePlot("h_p_z_LAB_2p", "$p_z(\hbox{2 protons})\ung{GeV}$");

GShipout("make_plots", hSkip=2mm, vSkip=0mm);

//----------------------------------------------------------------------------------------------------

NewPad("$\xi_1$", "$\xi_2$");
draw(RootGetObject(f, "before simulation/h_xi2_vs_xi1"));
limits((0, 0), (0.2, 0.2));
AttachLegend("no acceptance requirement");

NewPad("$\xi_1$", "$\xi_2$");
draw(RootGetObject(f, "after simulation/h_xi2_vs_xi1"));
limits((0, 0), (0.2, 0.2));
AttachLegend("protons in RP acceptance");

GShipout("xi2_vs_xi1", hSkip=2mm, vSkip=0mm);

//----------------------------------------------------------------------------------------------------

MakeOnePlot("h_angle_X_Z", "angle(X, Z)");

MakeOnePlot("h_angle_X_pr1_X_pr2", "angle(X product 1, X product 2)");

MakeOnePlot("h_angle_l_pl_l_mi", "angle($l^+$, $l^-$)");

GShipout("angles", hSkip=2mm, vSkip=0mm);

//----------------------------------------------------------------------------------------------------

write("finished");
