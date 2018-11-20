import root;
import pad_layout;

string files[], f_labels[];
files.push("data.root"); f_labels.push("data (fill 6031, DoubleEG)");
files.push("simu_apertures.root"); f_labels.push("simu, apertures");
//files.push("simu_no_apertures.root"); f_labels.push("simu, no apertures");

string rps[];
real rp_shx[], rp_shy[];
real x_marks[];
rps.push("23"); rp_shx.push(-41.8); rp_shy.push(3.5); x_marks.push(14.);
rps.push("3"); rp_shx.push(-3.7); rp_shy.push(3.7); x_marks.push(14.);
rps.push("103"); rp_shx.push(-2.8); rp_shy.push(3.75); x_marks.push(13.);
rps.push("123"); rp_shx.push(-42.1); rp_shy.push(3.5); x_marks.push(13.);

//----------------------------------------------------------------------------------------------------

for (int fi : files.keys)
{
	NewRow();

	NewPadLabel(f_labels[fi]);

	for (int rpi : rps.keys)
	{
		NewPad();
		scale(Linear, Linear, Log);
		
		RootObject obj = RootGetObject(files[fi], "RP " + rps[rpi] + "/h2_y_vs_x");
		obj.vExec("Rebin2D", 2, 2);

		bool data = (find(f_labels[fi], "data") >= 0);

		if (data)
			draw(shift(rp_shx[rpi], -rp_shy[rpi]), obj);
			//draw(shift(rp_shx[rpi], 0.), obj);
		else
			draw(obj);

		limits((0, -20.), (40., +20.), Crop);

		yaxis(XEquals(x_marks[rpi], false), magenta+1pt, above=true);
	}
}
