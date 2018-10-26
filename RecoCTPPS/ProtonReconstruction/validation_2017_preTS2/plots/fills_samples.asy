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
		// TODO

		return;
	}

	if (option == "short")
	{
		// TODO

		return;
	}
	AddDataSet("fill_5839/xangle_150");
	AddDataSet("fill_5840/xangle_150");
	AddDataSet("fill_5842/xangle_150");
	AddDataSet("fill_5845/xangle_150");
	AddDataSet("fill_5848/xangle_120");
	AddDataSet("fill_5848/xangle_150");
	AddDataSet("fill_5849/xangle_120");
	AddDataSet("fill_5849/xangle_150");
	AddDataSet("fill_5856/xangle_120");
	AddDataSet("fill_5856/xangle_150");
	AddDataSet("fill_5864/xangle_150");
	AddDataSet("fill_5865/xangle_150");
	AddDataSet("fill_5868/xangle_150");
	AddDataSet("fill_5942/xangle_150");
	AddDataSet("fill_5946/xangle_150");
	AddDataSet("fill_5950/xangle_120");
	AddDataSet("fill_5950/xangle_150");
	AddDataSet("fill_5952/xangle_150");
	AddDataSet("fill_5954/xangle_150");
	AddDataSet("fill_5958/xangle_150");
	AddDataSet("fill_5960/xangle_130");
	AddDataSet("fill_5960/xangle_140");
	AddDataSet("fill_5962/xangle_150");
	AddDataSet("fill_5963/xangle_150");
	AddDataSet("fill_5965/xangle_150");
	AddDataSet("fill_5966/xangle_150");
	AddDataSet("fill_5971/xangle_150");
	AddDataSet("fill_5974/xangle_150");
	AddDataSet("fill_5976/xangle_120");
	AddDataSet("fill_5979/xangle_120");
	AddDataSet("fill_5979/xangle_150");
	AddDataSet("fill_5984/xangle_150");
	AddDataSet("fill_6019/xangle_150");
	AddDataSet("fill_6020/xangle_150");
	AddDataSet("fill_6024/xangle_120");
	AddDataSet("fill_6024/xangle_150");
	AddDataSet("fill_6026/xangle_120");
	AddDataSet("fill_6026/xangle_150");
	AddDataSet("fill_6030/xangle_150");
	AddDataSet("fill_6031/xangle_120");
	AddDataSet("fill_6031/xangle_150");
	AddDataSet("fill_6035/xangle_120");
	AddDataSet("fill_6035/xangle_150");
	AddDataSet("fill_6041/xangle_150");
	AddDataSet("fill_6044/xangle_120");
	AddDataSet("fill_6044/xangle_150");
	AddDataSet("fill_6046/xangle_120");
	AddDataSet("fill_6046/xangle_150");
	AddDataSet("fill_6048/xangle_150");
	AddDataSet("fill_6050/xangle_120");
	AddDataSet("fill_6050/xangle_150");
	AddDataSet("fill_6052/xangle_120");
	AddDataSet("fill_6053/xangle_120");
	AddDataSet("fill_6053/xangle_150");
	AddDataSet("fill_6054/xangle_120");
	AddDataSet("fill_6054/xangle_150");
	AddDataSet("fill_6055/xangle_150");
	AddDataSet("fill_6057/xangle_150");
	AddDataSet("fill_6060/xangle_150");
	AddDataSet("fill_6061/xangle_120");
	AddDataSet("fill_6061/xangle_150");
	AddDataSet("fill_6084/xangle_150");
	AddDataSet("fill_6086/xangle_150");
	AddDataSet("fill_6089/xangle_150");
	AddDataSet("fill_6090/xangle_150");
	AddDataSet("fill_6091/xangle_150");
	AddDataSet("fill_6093/xangle_150");
	AddDataSet("fill_6094/xangle_150");
	AddDataSet("fill_6096/xangle_150");
	AddDataSet("fill_6097/xangle_120");
	AddDataSet("fill_6097/xangle_150");
	AddDataSet("fill_6098/xangle_150");
	AddDataSet("fill_6106/xangle_120");
	AddDataSet("fill_6106/xangle_150");
	AddDataSet("fill_6110/xangle_120");
	AddDataSet("fill_6110/xangle_150");
	AddDataSet("fill_6114/xangle_150");
	AddDataSet("fill_6116/xangle_150");
	AddDataSet("fill_6119/xangle_150");
	AddDataSet("fill_6123/xangle_150");
	AddDataSet("fill_6136/xangle_150");
	AddDataSet("fill_6138/xangle_150");
	AddDataSet("fill_6140/xangle_120");
	AddDataSet("fill_6140/xangle_150");
	AddDataSet("fill_6141/xangle_150");
	AddDataSet("fill_6142/xangle_120");
	AddDataSet("fill_6142/xangle_150");
	AddDataSet("fill_6143/xangle_120");
	AddDataSet("fill_6143/xangle_150");
	AddDataSet("fill_6146/xangle_150");
	AddDataSet("fill_6147/xangle_120");
	AddDataSet("fill_6147/xangle_150");
	AddDataSet("fill_6152/xangle_150");
	AddDataSet("fill_6155/xangle_150");
	AddDataSet("fill_6156/xangle_120");
	AddDataSet("fill_6156/xangle_150");
	AddDataSet("fill_6158/xangle_150");
	AddDataSet("fill_6159/xangle_150");
	AddDataSet("fill_6160/xangle_150");
	AddDataSet("fill_6161/xangle_120");
	AddDataSet("fill_6161/xangle_150");
	AddDataSet("fill_6165/xangle_120");
	AddDataSet("fill_6165/xangle_150");
	AddDataSet("fill_6167/xangle_150");
	AddDataSet("fill_6168/xangle_150");
	AddDataSet("fill_6169/xangle_150");
	AddDataSet("fill_6170/xangle_150");
	AddDataSet("fill_6171/xangle_150");
	AddDataSet("fill_6174/xangle_120");
	AddDataSet("fill_6174/xangle_150");
	AddDataSet("fill_6175/xangle_120");
	AddDataSet("fill_6175/xangle_150");
	AddDataSet("fill_6176/xangle_150");
	AddDataSet("fill_6177/xangle_120");
	AddDataSet("fill_6177/xangle_150");
	AddDataSet("fill_6179/xangle_150");
	AddDataSet("fill_6180/xangle_150");
	AddDataSet("fill_6182/xangle_120");
	AddDataSet("fill_6182/xangle_150");
	AddDataSet("fill_6185/xangle_150");
	AddDataSet("fill_6186/xangle_120");
	AddDataSet("fill_6186/xangle_150");
	AddDataSet("fill_6189/xangle_120");
	AddDataSet("fill_6189/xangle_150");
	AddDataSet("fill_6191/xangle_120");
	AddDataSet("fill_6191/xangle_150");
	AddDataSet("fill_6192/xangle_150");
	AddDataSet("fill_6193/xangle_150");

	AddDataSet("fill_5839/xangle_140");
	AddDataSet("fill_5840/xangle_140");
	AddDataSet("fill_5842/xangle_140");
	AddDataSet("fill_5848/xangle_140");
	AddDataSet("fill_5849/xangle_140");
	AddDataSet("fill_5856/xangle_140");
	AddDataSet("fill_5864/xangle_140");
	AddDataSet("fill_5865/xangle_140");
	AddDataSet("fill_5950/xangle_140");
	AddDataSet("fill_5954/xangle_140");
	AddDataSet("fill_5960/xangle_140");
	AddDataSet("fill_5962/xangle_140");
	AddDataSet("fill_5976/xangle_140");
	AddDataSet("fill_5979/xangle_140");
	AddDataSet("fill_6024/xangle_140");
	AddDataSet("fill_6026/xangle_140");
	AddDataSet("fill_6031/xangle_140");
	AddDataSet("fill_6035/xangle_140");
	AddDataSet("fill_6041/xangle_140");
	AddDataSet("fill_6044/xangle_140");
	AddDataSet("fill_6046/xangle_140");
	AddDataSet("fill_6050/xangle_140");
	AddDataSet("fill_6053/xangle_140");
	AddDataSet("fill_6054/xangle_140");
	AddDataSet("fill_6060/xangle_140");
	AddDataSet("fill_6061/xangle_140");
	AddDataSet("fill_6084/xangle_140");
	AddDataSet("fill_6086/xangle_140");
	AddDataSet("fill_6089/xangle_140");
	AddDataSet("fill_6090/xangle_140");
	AddDataSet("fill_6091/xangle_140");
	AddDataSet("fill_6093/xangle_140");
	AddDataSet("fill_6096/xangle_140");
	AddDataSet("fill_6097/xangle_140");
	AddDataSet("fill_6098/xangle_140");
	AddDataSet("fill_6106/xangle_140");
	AddDataSet("fill_6110/xangle_140");
	AddDataSet("fill_6119/xangle_140");
	AddDataSet("fill_6116/xangle_140");
	AddDataSet("fill_6136/xangle_140");
	AddDataSet("fill_6138/xangle_140");
	AddDataSet("fill_6140/xangle_140");
	AddDataSet("fill_6142/xangle_140");
	AddDataSet("fill_6143/xangle_140");
	AddDataSet("fill_6147/xangle_140");
	AddDataSet("fill_6152/xangle_140");
	AddDataSet("fill_6156/xangle_140");
	AddDataSet("fill_6158/xangle_140");
	AddDataSet("fill_6159/xangle_140");
	AddDataSet("fill_6160/xangle_140");
	AddDataSet("fill_6161/xangle_140");
	AddDataSet("fill_6165/xangle_140");
	AddDataSet("fill_6167/xangle_140");
	AddDataSet("fill_6168/xangle_140");
	AddDataSet("fill_6169/xangle_140");
	AddDataSet("fill_6170/xangle_140");
	AddDataSet("fill_6171/xangle_140");
	AddDataSet("fill_6174/xangle_140");
	AddDataSet("fill_6175/xangle_140");
	AddDataSet("fill_6177/xangle_140");
	AddDataSet("fill_6182/xangle_140");
	AddDataSet("fill_6185/xangle_140");
	AddDataSet("fill_6186/xangle_140");
	AddDataSet("fill_6189/xangle_140");
	AddDataSet("fill_6191/xangle_140");
	AddDataSet("fill_6192/xangle_140");
	AddDataSet("fill_6193/xangle_140");
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
