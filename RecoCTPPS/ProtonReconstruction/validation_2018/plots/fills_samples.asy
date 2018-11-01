struct Dataset
{
	string tag;
	string xangle;
	string beta;
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
	string xangle = substr(p, find(p, "xangle_")+7, 3);
	string beta = substr(p, find(p, "beta_")+5, 4);

	bool found = false;
	for (FillData fd : fill_data)
	{
		if (fd.fill == fill)
		{
			found = true;
			Dataset ds;
			ds.tag = p;
			ds.xangle = xangle;
			ds.beta = beta;
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
		ds.beta = beta;
		fd.datasets.push(ds);

		fill_data.push(fd);
	}
}

//----------------------------------------------------------------------------------------------------

void InitDataSets()
{
	/*
	AddDataSet("fill_6579/xangle_140_beta_0.30");
	AddDataSet("fill_6583/xangle_140_beta_0.30");
	AddDataSet("fill_6584/xangle_140_beta_0.30");
	AddDataSet("fill_6592/xangle_140_beta_0.30");
	AddDataSet("fill_6594/xangle_140_beta_0.30");
	AddDataSet("fill_6595/xangle_140_beta_0.30");
	AddDataSet("fill_6611/xangle_140_beta_0.30");
	AddDataSet("fill_6612/xangle_140_beta_0.30");
	AddDataSet("fill_6615/xangle_140_beta_0.30");
	AddDataSet("fill_6616/xangle_140_beta_0.30");
	AddDataSet("fill_6617/xangle_140_beta_0.30");
	AddDataSet("fill_6620/xangle_140_beta_0.30");
	AddDataSet("fill_6621/xangle_140_beta_0.30");
	AddDataSet("fill_6624/xangle_140_beta_0.30");
	AddDataSet("fill_6628/xangle_140_beta_0.30");
	AddDataSet("fill_6629/xangle_140_beta_0.30");
	AddDataSet("fill_6638/xangle_140_beta_0.30");
	AddDataSet("fill_6639/xangle_140_beta_0.30");
	AddDataSet("fill_6640/xangle_140_beta_0.30");
	AddDataSet("fill_6641/xangle_140_beta_0.30");
	AddDataSet("fill_6650/xangle_140_beta_0.30");
	AddDataSet("fill_6654/xangle_140_beta_0.30");
	AddDataSet("fill_6659/xangle_140_beta_0.30");
	AddDataSet("fill_6662/xangle_140_beta_0.30");
	AddDataSet("fill_6666/xangle_140_beta_0.30");
	AddDataSet("fill_6672/xangle_140_beta_0.30");
	AddDataSet("fill_6674/xangle_140_beta_0.30");
	AddDataSet("fill_6675/xangle_140_beta_0.30");
	AddDataSet("fill_6677/xangle_140_beta_0.30");
	AddDataSet("fill_6681/xangle_140_beta_0.30");
	AddDataSet("fill_6690/xangle_140_beta_0.30");
	AddDataSet("fill_6693/xangle_140_beta_0.30");
	AddDataSet("fill_6694/xangle_140_beta_0.30");
	AddDataSet("fill_6696/xangle_140_beta_0.30");
	AddDataSet("fill_6699/xangle_140_beta_0.30");
	AddDataSet("fill_6700/xangle_140_beta_0.30");
	AddDataSet("fill_6709/xangle_140_beta_0.30");
	AddDataSet("fill_6710/xangle_140_beta_0.30");
	AddDataSet("fill_6711/xangle_140_beta_0.30");
	AddDataSet("fill_6712/xangle_140_beta_0.30");
	AddDataSet("fill_6714/xangle_140_beta_0.30");
	AddDataSet("fill_6719/xangle_140_beta_0.30");
	AddDataSet("fill_6729/xangle_140_beta_0.30");
	AddDataSet("fill_6733/xangle_140_beta_0.30");
	AddDataSet("fill_6738/xangle_140_beta_0.30");
	AddDataSet("fill_6741/xangle_140_beta_0.30");
	*/
	AddDataSet("fill_6749/xangle_140_beta_0.30");
	AddDataSet("fill_6751/xangle_140_beta_0.30");
	AddDataSet("fill_6759/xangle_140_beta_0.30");
	AddDataSet("fill_6761/xangle_140_beta_0.30");
	AddDataSet("fill_6762/xangle_140_beta_0.30");
	AddDataSet("fill_6763/xangle_140_beta_0.30");
	AddDataSet("fill_6770/xangle_140_beta_0.30");
	AddDataSet("fill_6772/xangle_140_beta_0.30");
	AddDataSet("fill_6773/xangle_140_beta_0.30");
	AddDataSet("fill_6774/xangle_140_beta_0.30");
	AddDataSet("fill_6901/xangle_140_beta_0.30");
	AddDataSet("fill_6904/xangle_140_beta_0.30");
	AddDataSet("fill_6909/xangle_140_beta_0.30");
	AddDataSet("fill_6912/xangle_140_beta_0.30");
	AddDataSet("fill_6913/xangle_140_beta_0.30");
	AddDataSet("fill_6919/xangle_140_beta_0.30");
	AddDataSet("fill_6923/xangle_140_beta_0.30");
	AddDataSet("fill_6924/xangle_140_beta_0.30");
	AddDataSet("fill_6942/xangle_140_beta_0.30");
	AddDataSet("fill_6956/xangle_140_beta_0.30");
	AddDataSet("fill_6957/xangle_140_beta_0.30");
	AddDataSet("fill_6960/xangle_140_beta_0.30");
	AddDataSet("fill_6961/xangle_140_beta_0.30");
	AddDataSet("fill_6966/xangle_140_beta_0.30");
	AddDataSet("fill_6998/xangle_140_beta_0.30");
	AddDataSet("fill_7005/xangle_140_beta_0.30");
	AddDataSet("fill_7006/xangle_140_beta_0.30");
	AddDataSet("fill_7017/xangle_140_beta_0.30");
	AddDataSet("fill_7018/xangle_140_beta_0.30");
	AddDataSet("fill_7020/xangle_140_beta_0.30");
	AddDataSet("fill_7024/xangle_140_beta_0.30");
	AddDataSet("fill_7031/xangle_140_beta_0.30");
	AddDataSet("fill_7037/xangle_140_beta_0.30");
	AddDataSet("fill_7039/xangle_140_beta_0.30");
	AddDataSet("fill_7040/xangle_140_beta_0.30");
	AddDataSet("fill_7042/xangle_140_beta_0.30");
	AddDataSet("fill_7043/xangle_140_beta_0.30");
	AddDataSet("fill_7045/xangle_140_beta_0.30");
	AddDataSet("fill_7047/xangle_140_beta_0.30");
	AddDataSet("fill_7052/xangle_140_beta_0.30");
	AddDataSet("fill_7053/xangle_140_beta_0.30");
	AddDataSet("fill_7055/xangle_140_beta_0.30");
	AddDataSet("fill_7056/xangle_140_beta_0.30");
	AddDataSet("fill_7058/xangle_140_beta_0.30");
	AddDataSet("fill_7061/xangle_140_beta_0.30");
	AddDataSet("fill_7063/xangle_140_beta_0.30");
	AddDataSet("fill_7065/xangle_140_beta_0.30");
	AddDataSet("fill_7069/xangle_140_beta_0.30");
	AddDataSet("fill_7080/xangle_140_beta_0.30");
	AddDataSet("fill_7083/xangle_140_beta_0.30");
	AddDataSet("fill_7087/xangle_140_beta_0.30");
	AddDataSet("fill_7090/xangle_140_beta_0.30");
	AddDataSet("fill_7091/xangle_140_beta_0.30");
	AddDataSet("fill_7095/xangle_140_beta_0.30");
	AddDataSet("fill_7097/xangle_140_beta_0.30");
	AddDataSet("fill_7105/xangle_140_beta_0.30");
	AddDataSet("fill_7108/xangle_140_beta_0.30");
	AddDataSet("fill_7110/xangle_140_beta_0.30");
	AddDataSet("fill_7118/xangle_140_beta_0.30");
	AddDataSet("fill_7122/xangle_140_beta_0.30");
	AddDataSet("fill_7123/xangle_140_beta_0.30");
	AddDataSet("fill_7124/xangle_140_beta_0.30");
	AddDataSet("fill_7125/xangle_140_beta_0.30");
	AddDataSet("fill_7127/xangle_140_beta_0.30");
	AddDataSet("fill_7128/xangle_140_beta_0.30");
	AddDataSet("fill_7131/xangle_140_beta_0.30");
	AddDataSet("fill_7137/xangle_140_beta_0.30");
	AddDataSet("fill_7139/xangle_140_beta_0.30");
	AddDataSet("fill_7144/xangle_140_beta_0.30");
	AddDataSet("fill_7145/xangle_140_beta_0.30");
	AddDataSet("fill_6931/xangle_140_beta_0.30");
	AddDataSet("fill_6940/xangle_140_beta_0.30");
	AddDataSet("fill_7309/xangle_140_beta_0.30");
	AddDataSet("fill_7310/xangle_140_beta_0.30");
	AddDataSet("fill_7314/xangle_140_beta_0.30");
	AddDataSet("fill_7315/xangle_140_beta_0.30");
	AddDataSet("fill_7320/xangle_140_beta_0.30");
	AddDataSet("fill_7321/xangle_140_beta_0.30");
	AddDataSet("fill_7324/xangle_140_beta_0.30");
	AddDataSet("fill_7328/xangle_140_beta_0.30");
	AddDataSet("fill_7333/xangle_140_beta_0.30");
	AddDataSet("fill_7334/xangle_140_beta_0.30");
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
