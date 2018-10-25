struct Dataset
{
	string tag;
	int xangle;
}

//----------------------------------------------------------------------------------------------------

struct FillData
{
	int fill;
	Dataset datasets[];
};

//----------------------------------------------------------------------------------------------------

FillData fill_data[];

void AddDataSet(string p)
{
	int fill = (int) substr(p, find(p, "fill_")+5, 4);
	int xangle = (int) substr(p, find(p, "xangle_")+7, 3);

	bool found = false;
	for (FillData fd : fill_data)
	{
		if (fd.fill == fill)
		{
			found = true;
			Dataset ds;
			ds.tag = p;
			ds.xangle = xangle;
			fd.datasets.push(ds);
		}
	}

	if (!found)
	{
		FillData fd;
		fd.fill = fill;
		Dataset ds;
		ds.tag = p;
		ds.xangle = xangle;
		fd.datasets.push(ds);

		fill_data.push(fd);
	}
}

//----------------------------------------------------------------------------------------------------

void InitDataSets(string option="all")
{
	if (option == "very_short")
	{
		AddDataSet("phys_margin/fill_4947");
		AddDataSet("phys_no_margin/fill_5288");

		return;
	}

	if (option == "short")
	{
		AddDataSet("phys_margin/fill_4947");
		AddDataSet("phys_no_margin/fill_4988");
		AddDataSet("phys_no_margin/fill_5027");
		AddDataSet("phys_no_margin/fill_5052");
		AddDataSet("phys_no_margin/fill_5261");
		AddDataSet("phys_no_margin/fill_5275");
		AddDataSet("phys_no_margin/fill_5288");

		return;
	}

	AddDataSet("fill_6239/xangle_150");
	AddDataSet("fill_6240/xangle_130");
	AddDataSet("fill_6240/xangle_150");
	AddDataSet("fill_6241/xangle_130");
	AddDataSet("fill_6241/xangle_150");
	AddDataSet("fill_6243/xangle_130");
	AddDataSet("fill_6243/xangle_150");
	AddDataSet("fill_6245/xangle_130");
	AddDataSet("fill_6245/xangle_150");
	AddDataSet("fill_6247/xangle_130");
	AddDataSet("fill_6247/xangle_150");
	AddDataSet("fill_6252/xangle_130");
	AddDataSet("fill_6252/xangle_150");
	AddDataSet("fill_6253/xangle_110");
	AddDataSet("fill_6253/xangle_130");
	AddDataSet("fill_6255/xangle_110");
	AddDataSet("fill_6255/xangle_130");
	AddDataSet("fill_6255/xangle_150");
	AddDataSet("fill_6258/xangle_110");
	AddDataSet("fill_6258/xangle_130");
	AddDataSet("fill_6258/xangle_150");
	AddDataSet("fill_6259/xangle_110");
	AddDataSet("fill_6259/xangle_130");
	AddDataSet("fill_6259/xangle_150");
	AddDataSet("fill_6261/xangle_130");
	AddDataSet("fill_6262/xangle_130");
	AddDataSet("fill_6263/xangle_130");
	AddDataSet("fill_6263/xangle_150");
	AddDataSet("fill_6266/xangle_110");
	AddDataSet("fill_6266/xangle_130");
	AddDataSet("fill_6266/xangle_150");
	AddDataSet("fill_6268/xangle_130");
	AddDataSet("fill_6268/xangle_150");
	AddDataSet("fill_6269/xangle_150");
	AddDataSet("fill_6271/xangle_130");
	AddDataSet("fill_6271/xangle_150");
	AddDataSet("fill_6272/xangle_110");
	AddDataSet("fill_6272/xangle_130");
	AddDataSet("fill_6272/xangle_150");
	AddDataSet("fill_6275/xangle_130");
	AddDataSet("fill_6275/xangle_150");
	AddDataSet("fill_6276/xangle_130");
	AddDataSet("fill_6276/xangle_150");
	AddDataSet("fill_6278/xangle_130");
	AddDataSet("fill_6278/xangle_150");
	AddDataSet("fill_6279/xangle_130");
	AddDataSet("fill_6279/xangle_150");
	AddDataSet("fill_6283/xangle_130");
	AddDataSet("fill_6283/xangle_150");
	AddDataSet("fill_6284/xangle_130");
	AddDataSet("fill_6284/xangle_150");
	AddDataSet("fill_6285/xangle_130");
	AddDataSet("fill_6285/xangle_150");
	AddDataSet("fill_6287/xangle_110");
	AddDataSet("fill_6287/xangle_130");
	AddDataSet("fill_6287/xangle_150");
	AddDataSet("fill_6288/xangle_130");
	AddDataSet("fill_6288/xangle_150");
	AddDataSet("fill_6291/xangle_110");
	AddDataSet("fill_6291/xangle_130");
	AddDataSet("fill_6291/xangle_150");
	AddDataSet("fill_6297/xangle_110");
	AddDataSet("fill_6297/xangle_130");
	AddDataSet("fill_6297/xangle_150");
	AddDataSet("fill_6298/xangle_110");
	AddDataSet("fill_6298/xangle_130");
	AddDataSet("fill_6298/xangle_150");
	AddDataSet("fill_6300/xangle_130");
	AddDataSet("fill_6300/xangle_150");
	AddDataSet("fill_6303/xangle_130");
	AddDataSet("fill_6303/xangle_150");
	AddDataSet("fill_6304/xangle_130");
	AddDataSet("fill_6304/xangle_150");
	AddDataSet("fill_6305/xangle_130");
	AddDataSet("fill_6305/xangle_150");
	AddDataSet("fill_6306/xangle_130");
	AddDataSet("fill_6306/xangle_150");
	AddDataSet("fill_6307/xangle_130");
	AddDataSet("fill_6307/xangle_150");
	AddDataSet("fill_6308/xangle_130");
	AddDataSet("fill_6308/xangle_150");
	AddDataSet("fill_6309/xangle_150");
	AddDataSet("fill_6311/xangle_130");
	AddDataSet("fill_6311/xangle_150");
	AddDataSet("fill_6312/xangle_130");
	AddDataSet("fill_6312/xangle_150");
	AddDataSet("fill_6313/xangle_150");
	AddDataSet("fill_6314/xangle_130");
	AddDataSet("fill_6314/xangle_150");
	AddDataSet("fill_6315/xangle_130");
	AddDataSet("fill_6315/xangle_150");
	AddDataSet("fill_6317/xangle_130");
	AddDataSet("fill_6317/xangle_150");
	AddDataSet("fill_6318/xangle_130");
	AddDataSet("fill_6318/xangle_150");
	AddDataSet("fill_6323/xangle_130");
	AddDataSet("fill_6323/xangle_150");
	AddDataSet("fill_6324/xangle_110");
	AddDataSet("fill_6324/xangle_130");
	AddDataSet("fill_6324/xangle_150");
	AddDataSet("fill_6325/xangle_130");
	AddDataSet("fill_6325/xangle_150");
	AddDataSet("fill_6337/xangle_130");
	AddDataSet("fill_6337/xangle_150");
	AddDataSet("fill_6341/xangle_150");
	AddDataSet("fill_6343/xangle_130");
	AddDataSet("fill_6343/xangle_150");
	AddDataSet("fill_6344/xangle_130");
	AddDataSet("fill_6344/xangle_150");
	AddDataSet("fill_6346/xangle_130");
	AddDataSet("fill_6346/xangle_150");
	AddDataSet("fill_6347/xangle_130");
	AddDataSet("fill_6347/xangle_150");
	AddDataSet("fill_6348/xangle_130");
	AddDataSet("fill_6348/xangle_150");
	AddDataSet("fill_6349/xangle_150");
	AddDataSet("fill_6351/xangle_150");
	AddDataSet("fill_6355/xangle_150");
	AddDataSet("fill_6356/xangle_130");
	AddDataSet("fill_6356/xangle_150");
	AddDataSet("fill_6358/xangle_130");
	AddDataSet("fill_6360/xangle_130");
	AddDataSet("fill_6360/xangle_150");
	AddDataSet("fill_6362/xangle_130");
	AddDataSet("fill_6362/xangle_150");
	AddDataSet("fill_6364/xangle_130");
	AddDataSet("fill_6364/xangle_150");
	AddDataSet("fill_6370/xangle_130");
	AddDataSet("fill_6370/xangle_150");
	AddDataSet("fill_6371/xangle_130");
	AddDataSet("fill_6371/xangle_150");

	AddDataSet("fill_6240/xangle_140");
	AddDataSet("fill_6241/xangle_140");
	AddDataSet("fill_6243/xangle_140");
	AddDataSet("fill_6245/xangle_140");
	AddDataSet("fill_6247/xangle_140");
	AddDataSet("fill_6252/xangle_140");
	AddDataSet("fill_6253/xangle_140");
	AddDataSet("fill_6255/xangle_140");
	AddDataSet("fill_6258/xangle_140");
	AddDataSet("fill_6259/xangle_140");
	AddDataSet("fill_6261/xangle_140");
	AddDataSet("fill_6262/xangle_140");
	AddDataSet("fill_6263/xangle_140");
	AddDataSet("fill_6266/xangle_140");
	AddDataSet("fill_6268/xangle_140");
	AddDataSet("fill_6271/xangle_140");
	AddDataSet("fill_6272/xangle_140");
	AddDataSet("fill_6275/xangle_140");
	AddDataSet("fill_6276/xangle_140");
	AddDataSet("fill_6278/xangle_140");
	AddDataSet("fill_6279/xangle_140");
	AddDataSet("fill_6283/xangle_140");
	AddDataSet("fill_6284/xangle_140");
	AddDataSet("fill_6285/xangle_140");
	AddDataSet("fill_6287/xangle_140");
	AddDataSet("fill_6288/xangle_140");
	AddDataSet("fill_6291/xangle_140");
	AddDataSet("fill_6297/xangle_140");
	AddDataSet("fill_6298/xangle_140");
	AddDataSet("fill_6300/xangle_140");
	AddDataSet("fill_6303/xangle_140");
	AddDataSet("fill_6304/xangle_140");
	AddDataSet("fill_6305/xangle_140");
	AddDataSet("fill_6306/xangle_140");
	AddDataSet("fill_6307/xangle_140");
	AddDataSet("fill_6308/xangle_140");
	AddDataSet("fill_6311/xangle_140");
	AddDataSet("fill_6312/xangle_140");
	AddDataSet("fill_6314/xangle_140");
	AddDataSet("fill_6315/xangle_140");
	AddDataSet("fill_6318/xangle_140");
	AddDataSet("fill_6324/xangle_140");
	AddDataSet("fill_6325/xangle_140");
	AddDataSet("fill_6337/xangle_140");
	AddDataSet("fill_6343/xangle_140");
	AddDataSet("fill_6344/xangle_140");
	AddDataSet("fill_6347/xangle_140");
	AddDataSet("fill_6348/xangle_140");
	AddDataSet("fill_6349/xangle_140");
	AddDataSet("fill_6356/xangle_140");
	AddDataSet("fill_6358/xangle_140");
	AddDataSet("fill_6360/xangle_140");
	AddDataSet("fill_6362/xangle_140");
	AddDataSet("fill_6364/xangle_140");
	AddDataSet("fill_6370/xangle_140");
	AddDataSet("fill_6371/xangle_140");
}

//----------------------------------------------------------------------------------------------------

mark GetDatasetMark(Dataset d)
{
	return mCi+3pt+true;
}

//----------------------------------------------------------------------------------------------------

pen GetDatasetPen(Dataset d)
{
	return red;
}
