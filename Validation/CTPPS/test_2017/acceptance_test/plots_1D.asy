import root;
import pad_layout;

string f = "acceptance_xangle_140.root";

xSizeDef = 10cm;
ySizeDef = 8cm;

//----------------------------------------------------------------------------------------------------

NewPad("$\xi$", "acceptance");
draw(RootGetObject(f, "3/h_xi_rat"), "vl", blue, "near");
draw(RootGetObject(f, "23/h_xi_rat"), "vl", red, "far");
draw(RootGetObject(f, "3,23/h_xi_rat"), "vl", heavygreen, "near \& far");
limits((0, 0), (0.25, 1.), Crop);
AttachLegend("sector 45");


NewPad("$\xi$", "acceptance");
draw(RootGetObject(f, "103/h_xi_rat"), "vl", blue, "near");
draw(RootGetObject(f, "123/h_xi_rat"), "vl", red, "far");
draw(RootGetObject(f, "103,123/h_xi_rat"), "vl", heavygreen, "near \& far");
limits((0, 0), (0.25, 1.), Crop);
AttachLegend("sector 56");


NewPad("$m\ung{GeV}$", "acceptance");
draw(RootGetObject(f, "3,103/h_m_rat"), "vl", blue, "near");
draw(RootGetObject(f, "23,123/h_m_rat"), "vl", red, "far");
draw(RootGetObject(f, "3,23,103,123/h_m_rat"), "vl", heavygreen, "near \& far");
AttachLegend("sector 45 \& 56");
