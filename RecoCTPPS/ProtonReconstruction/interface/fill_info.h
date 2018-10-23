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
    // 2016 pre-TS2
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

    // 2017 pre-TS2
	fillInfoCollection.push_back(FillInfo(0, false, 297046, 302663, "2017_preTS2/preliminary"));

    // 2017 post-TS2
	fillInfoCollection.push_back(FillInfo(6230, false, 303719, 303729, "fill 6230"));
	fillInfoCollection.push_back(FillInfo(6236, false, 303790, 303795, "fill 6236"));
	fillInfoCollection.push_back(FillInfo(6238, false, 303817, 303819, "fill 6238"));
	fillInfoCollection.push_back(FillInfo(6239, false, 303824, 303825, "fill 6239"));
	fillInfoCollection.push_back(FillInfo(6240, false, 303832, 303832, "fill 6240"));
	fillInfoCollection.push_back(FillInfo(6241, false, 303838, 303838, "fill 6241"));
	fillInfoCollection.push_back(FillInfo(6243, false, 303885, 303885, "fill 6243"));
	fillInfoCollection.push_back(FillInfo(6245, false, 303948, 303989, "fill 6245"));
	fillInfoCollection.push_back(FillInfo(6247, false, 303998, 304000, "fill 6247"));
	fillInfoCollection.push_back(FillInfo(6252, false, 304062, 304062, "fill 6252"));
	fillInfoCollection.push_back(FillInfo(6253, false, 304119, 304125, "fill 6253"));
	fillInfoCollection.push_back(FillInfo(6255, false, 304144, 304144, "fill 6255"));
	fillInfoCollection.push_back(FillInfo(6258, false, 304158, 304158, "fill 6258"));
	fillInfoCollection.push_back(FillInfo(6259, false, 304169, 304170, "fill 6259"));
	fillInfoCollection.push_back(FillInfo(6261, false, 304196, 304200, "fill 6261"));
	fillInfoCollection.push_back(FillInfo(6262, false, 304204, 304204, "fill 6262"));
	fillInfoCollection.push_back(FillInfo(6263, false, 304209, 304209, "fill 6263"));
	fillInfoCollection.push_back(FillInfo(6266, false, 304291, 304292, "fill 6266"));
	fillInfoCollection.push_back(FillInfo(6268, false, 304333, 304333, "fill 6268"));
	fillInfoCollection.push_back(FillInfo(6269, false, 304354, 304354, "fill 6269"));
	fillInfoCollection.push_back(FillInfo(6271, false, 304366, 304366, "fill 6271"));
	fillInfoCollection.push_back(FillInfo(6272, false, 304446, 304452, "fill 6272"));
	fillInfoCollection.push_back(FillInfo(6275, false, 304505, 304508, "fill 6275"));
	fillInfoCollection.push_back(FillInfo(6276, false, 304562, 304562, "fill 6276"));
	fillInfoCollection.push_back(FillInfo(6278, false, 304616, 304616, "fill 6278"));
	fillInfoCollection.push_back(FillInfo(6279, false, 304625, 304626, "fill 6279"));
	fillInfoCollection.push_back(FillInfo(6283, false, 304654, 304655, "fill 6283"));
	fillInfoCollection.push_back(FillInfo(6284, false, 304661, 304663, "fill 6284"));
	fillInfoCollection.push_back(FillInfo(6285, false, 304671, 304672, "fill 6285"));
	fillInfoCollection.push_back(FillInfo(6287, false, 304737, 304740, "fill 6287"));
	fillInfoCollection.push_back(FillInfo(6288, false, 304776, 304778, "fill 6288"));
	fillInfoCollection.push_back(FillInfo(6291, false, 304797, 304797, "fill 6291"));
	fillInfoCollection.push_back(FillInfo(6294, false, 304880, 304880, "fill 6294"));
	fillInfoCollection.push_back(FillInfo(6295, false, 304899, 304907, "fill 6295"));
	fillInfoCollection.push_back(FillInfo(6297, false, 305040, 305046, "fill 6297"));
	fillInfoCollection.push_back(FillInfo(6298, false, 305059, 305064, "fill 6298"));
	fillInfoCollection.push_back(FillInfo(6300, false, 305081, 305081, "fill 6300"));
	fillInfoCollection.push_back(FillInfo(6303, false, 305112, 305114, "fill 6303"));
	fillInfoCollection.push_back(FillInfo(6304, false, 305178, 305188, "fill 6304"));
	fillInfoCollection.push_back(FillInfo(6305, false, 305202, 305204, "fill 6305"));
	fillInfoCollection.push_back(FillInfo(6306, false, 305207, 305208, "fill 6306"));
	fillInfoCollection.push_back(FillInfo(6307, false, 305234, 305237, "fill 6307"));
	fillInfoCollection.push_back(FillInfo(6308, false, 305247, 305252, "fill 6308"));
	fillInfoCollection.push_back(FillInfo(6309, false, 305282, 305282, "fill 6309"));
	fillInfoCollection.push_back(FillInfo(6311, false, 305310, 305314, "fill 6311"));
	fillInfoCollection.push_back(FillInfo(6312, false, 305336, 305351, "fill 6312"));
	fillInfoCollection.push_back(FillInfo(6313, false, 305358, 305358, "fill 6313"));
	fillInfoCollection.push_back(FillInfo(6314, false, 305364, 305367, "fill 6314"));
	fillInfoCollection.push_back(FillInfo(6315, false, 305376, 305377, "fill 6315"));
	fillInfoCollection.push_back(FillInfo(6317, false, 305405, 305406, "fill 6317"));
	fillInfoCollection.push_back(FillInfo(6318, false, 305440, 305441, "fill 6318"));
	fillInfoCollection.push_back(FillInfo(6323, false, 305516, 305518, "fill 6323"));
	fillInfoCollection.push_back(FillInfo(6324, false, 305586, 305590, "fill 6324"));
	fillInfoCollection.push_back(FillInfo(6325, false, 305636, 305636, "fill 6325"));
	fillInfoCollection.push_back(FillInfo(6336, false, 305745, 305758, "fill 6336"));
	fillInfoCollection.push_back(FillInfo(6337, false, 305766, 305766, "fill 6337"));
	fillInfoCollection.push_back(FillInfo(6341, false, 305809, 305809, "fill 6341"));
	fillInfoCollection.push_back(FillInfo(6343, false, 305814, 305814, "fill 6343"));
	fillInfoCollection.push_back(FillInfo(6344, false, 305821, 305821, "fill 6344"));
	fillInfoCollection.push_back(FillInfo(6346, false, 305832, 305842, "fill 6346"));
	fillInfoCollection.push_back(FillInfo(6347, false, 305862, 305862, "fill 6347"));
	fillInfoCollection.push_back(FillInfo(6348, false, 305898, 305898, "fill 6348"));
	fillInfoCollection.push_back(FillInfo(6349, false, 305902, 305902, "fill 6349"));
	fillInfoCollection.push_back(FillInfo(6351, false, 305965, 305967, "fill 6351"));
	fillInfoCollection.push_back(FillInfo(6355, false, 306029, 306030, "fill 6355"));
	fillInfoCollection.push_back(FillInfo(6356, false, 306036, 306051, "fill 6356"));
	fillInfoCollection.push_back(FillInfo(6358, false, 306091, 306096, "fill 6358"));
	fillInfoCollection.push_back(FillInfo(6360, false, 306121, 306126, "fill 6360"));
	fillInfoCollection.push_back(FillInfo(6362, false, 306134, 306139, "fill 6362"));
	fillInfoCollection.push_back(FillInfo(6364, false, 306153, 306171, "fill 6364"));
	fillInfoCollection.push_back(FillInfo(6370, false, 306416, 306432, "fill 6370"));
	fillInfoCollection.push_back(FillInfo(6371, false, 306454, 306462, "fill 6371"));
}

#endif
