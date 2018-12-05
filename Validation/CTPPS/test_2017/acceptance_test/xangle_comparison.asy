import root;
import pad_layout;

string files[], f_labels[];
pen f_pens[];
files.push("acceptance_xangle_130.root"); f_labels.push("xangle 130"); f_pens.push(red);
files.push("acceptance_xangle_140.root"); f_labels.push("xangle 140"); f_pens.push(blue);
files.push("acceptance_xangle_150.root"); f_labels.push("xangle 150"); f_pens.push(heavygreen);

xSizeDef = 10cm;
ySizeDef = 8cm;

//----------------------------------------------------------------------------------------------------

NewPad("$\xi$", "acceptance");

for (int fi : files.keys)
	draw(RootGetObject(files[fi], "3,23/h_xi_rat"), "vl", f_pens[fi], f_labels[fi]);

limits((0, 0), (0.25, 1.), Crop);
AttachLegend("sector 45, near \& far");

//----------------------------------------------------------------------------------------------------

NewPad("$\xi$", "acceptance");

for (int fi : files.keys)
	draw(RootGetObject(files[fi], "103,123/h_xi_rat"), "vl", f_pens[fi], f_labels[fi]);

limits((0, 0), (0.25, 1.), Crop);
AttachLegend("sector 56, near \& far");

//----------------------------------------------------------------------------------------------------

NewPad("$m\ung{GeV}$", "acceptance");

for (int fi : files.keys)
	draw(RootGetObject(files[fi], "3,23,103,123/h_m_rat"), "vl", f_pens[fi], f_labels[fi]);

AttachLegend("sector 45 \& 56, near \& far");
