struct Dataset
{
	string tag;
	bool margin;
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
	bool margin = (find(p, "phys_margin") == 0);

	bool found = false;
	for (FillData fd : fill_data)
	{
		if (fd.fill == fill)
		{
			found = true;
			Dataset ds;
			ds.tag = p;
			ds.margin = margin;
			fd.datasets.push(ds);
		}
	}

	if (!found)
	{
		FillData fd;
		fd.fill = fill;
		Dataset ds;
		ds.tag = p;
		ds.margin = margin;
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

	AddDataSet("phys_margin/fill_4947");
	AddDataSet("phys_margin/fill_4953");
	AddDataSet("phys_margin/fill_4961");
	AddDataSet("phys_margin/fill_4964");
	AddDataSet("phys_margin/fill_4976");

	AddDataSet("phys_no_margin/fill_4964");
	AddDataSet("phys_no_margin/fill_4985");
	AddDataSet("phys_no_margin/fill_4988");
	AddDataSet("phys_no_margin/fill_4990");
	AddDataSet("phys_no_margin/fill_5005");
	AddDataSet("phys_no_margin/fill_5013");
	AddDataSet("phys_no_margin/fill_5017");
	AddDataSet("phys_no_margin/fill_5020");
	AddDataSet("phys_no_margin/fill_5021");
	AddDataSet("phys_no_margin/fill_5024");
	AddDataSet("phys_no_margin/fill_5026");
	AddDataSet("phys_no_margin/fill_5027");
	AddDataSet("phys_no_margin/fill_5028");
	AddDataSet("phys_no_margin/fill_5029");
	AddDataSet("phys_no_margin/fill_5030");
	AddDataSet("phys_no_margin/fill_5038");
	AddDataSet("phys_no_margin/fill_5043");
	AddDataSet("phys_no_margin/fill_5045");
	AddDataSet("phys_no_margin/fill_5048");
	AddDataSet("phys_no_margin/fill_5052");
	AddDataSet("phys_no_margin/fill_5261");
	AddDataSet("phys_no_margin/fill_5264");
	AddDataSet("phys_no_margin/fill_5265");
	AddDataSet("phys_no_margin/fill_5266");
	AddDataSet("phys_no_margin/fill_5267");
	AddDataSet("phys_no_margin/fill_5274");
	AddDataSet("phys_no_margin/fill_5275");
	AddDataSet("phys_no_margin/fill_5276");
	AddDataSet("phys_no_margin/fill_5277");
	AddDataSet("phys_no_margin/fill_5279");
	AddDataSet("phys_no_margin/fill_5287");
	AddDataSet("phys_no_margin/fill_5288");
}

//----------------------------------------------------------------------------------------------------

mark GetDatasetMark(Dataset d)
{
	if (d.margin)
		return mSq+4pt+false;
	else
		return mCi+3pt+true;
}

//----------------------------------------------------------------------------------------------------

pen GetDatasetPen(Dataset d)
{
	if (d.margin)
		return blue;
	else
		return red;
}
