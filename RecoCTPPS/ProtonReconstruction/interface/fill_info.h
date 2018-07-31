#ifndef _fill_info_h_
#define _fill_info_h_

#include <vector>
#include <string>

//----------------------------------------------------------------------------------------------------

struct FillInfo
{
	unsigned int fillNumber;
	bool margin;
	unsigned int runMin, runMax;
	std::string alignmentTag;

	FillInfo(unsigned int _fn=0, bool _m=false, unsigned int _rmin=0, unsigned int _rmax=0, const std::string &at="") :
		fillNumber(_fn), margin(_m), runMin(_rmin), runMax(_rmax), alignmentTag(at)
	{
	}
};

//----------------------------------------------------------------------------------------------------

struct FillInfoCollection : public std::vector<FillInfo> 
{
	unsigned int FindByFill(unsigned int fill, bool margin, FillInfo &fillInfo) const
	{
		for (const auto it : *this)
		{
			if (it.fillNumber == fill && it.margin == margin)
            {
				fillInfo = it;
                return 0;
            }
		}

        return 1;
	}

	unsigned int FindByRun(unsigned int run, FillInfo &fillInfo) const
	{
		for (const auto it : *this)
		{
			if (it.runMin <= run && it.runMax >= run)
            {
				fillInfo = it;
                return 0;
            }
		}

		return 1;
	}
};

//----------------------------------------------------------------------------------------------------

FillInfoCollection fillInfoCollection;

//----------------------------------------------------------------------------------------------------

void InitFillInfoCollection()
{
	fillInfoCollection.push_back(FillInfo(4947, true, 273725, 273730, "phys_margin/fill_4947"));
	fillInfoCollection.push_back(FillInfo(4953, true, 274094, 274094, "phys_margin/fill_4953"));
	fillInfoCollection.push_back(FillInfo(4961, true, 274198, 274200, "phys_margin/fill_4961"));
	fillInfoCollection.push_back(FillInfo(4964, true, 274240, 274241, "phys_margin/fill_4964"));
	fillInfoCollection.push_back(FillInfo(4964, false, 274244, 274244, "phys_no_margin/fill_4964"));
	fillInfoCollection.push_back(FillInfo(4976, true, 274282, 274286, "phys_margin/fill_4976"));
	fillInfoCollection.push_back(FillInfo(4985, false, 274387, 274388, "phys_no_margin/fill_4985"));
	fillInfoCollection.push_back(FillInfo(4988, false, 274420, 274422, "phys_no_margin/fill_4988"));
	fillInfoCollection.push_back(FillInfo(4990, false, 274440, 274443, "phys_no_margin/fill_4990"));
	fillInfoCollection.push_back(FillInfo(5005, false, 274954, 274959, "phys_no_margin/fill_5005"));
	fillInfoCollection.push_back(FillInfo(5013, false, 274966, 274971, "phys_no_margin/fill_5013"));
	fillInfoCollection.push_back(FillInfo(5017, false, 274998, 275001, "phys_no_margin/fill_5017"));
	fillInfoCollection.push_back(FillInfo(5020, false, 275059, 275074, "phys_no_margin/fill_5020"));
	fillInfoCollection.push_back(FillInfo(5021, false, 275124, 275125, "phys_no_margin/fill_5021"));
	fillInfoCollection.push_back(FillInfo(5024, false, 275282, 275293, "phys_no_margin/fill_5024"));
	fillInfoCollection.push_back(FillInfo(5026, false, 275309, 275311, "phys_no_margin/fill_5026"));
	fillInfoCollection.push_back(FillInfo(5027, false, 275319, 275338, "phys_no_margin/fill_5027"));
	fillInfoCollection.push_back(FillInfo(5028, false, 275344, 275345, "phys_no_margin/fill_5028"));
	fillInfoCollection.push_back(FillInfo(5029, false, 275370, 275371, "phys_no_margin/fill_5029"));
	fillInfoCollection.push_back(FillInfo(5030, false, 275375, 275376, "phys_no_margin/fill_5030"));
	fillInfoCollection.push_back(FillInfo(5038, false, 275656, 275659, "phys_no_margin/fill_5038"));
	fillInfoCollection.push_back(FillInfo(5043, false, 275757, 275783, "phys_no_margin/fill_5043"));
	fillInfoCollection.push_back(FillInfo(5045, false, 275828, 275847, "phys_no_margin/fill_5045"));
	fillInfoCollection.push_back(FillInfo(5048, false, 275886, 275890, "phys_no_margin/fill_5048"));
	fillInfoCollection.push_back(FillInfo(5052, false, 275911, 275931, "phys_no_margin/fill_5052"));

	fillInfoCollection.push_back(FillInfo(5261, false, 279760, 279767, "phys_no_margin/fill_5261"));
	fillInfoCollection.push_back(FillInfo(5264, false, 279794, 279794, "phys_no_margin/fill_5264"));
	fillInfoCollection.push_back(FillInfo(5265, false, 279823, 279823, "phys_no_margin/fill_5265"));
	fillInfoCollection.push_back(FillInfo(5266, false, 279841, 279841, "phys_no_margin/fill_5266"));
	fillInfoCollection.push_back(FillInfo(5267, false, 279844, 279865, "phys_no_margin/fill_5267"));
	fillInfoCollection.push_back(FillInfo(5274, false, 279931, 279931, "phys_no_margin/fill_5274"));
	fillInfoCollection.push_back(FillInfo(5275, false, 279966, 279966, "phys_no_margin/fill_5275"));
	fillInfoCollection.push_back(FillInfo(5276, false, 279975, 279975, "phys_no_margin/fill_5276"));
	fillInfoCollection.push_back(FillInfo(5277, false, 279993, 280024, "phys_no_margin/fill_5277"));
	fillInfoCollection.push_back(FillInfo(5279, false, 280187, 280194, "phys_no_margin/fill_5279"));
	fillInfoCollection.push_back(FillInfo(5287, false, 280327, 280364, "phys_no_margin/fill_5287"));
	fillInfoCollection.push_back(FillInfo(5288, false, 280383, 280385, "phys_no_margin/fill_5288"));
}

#endif
