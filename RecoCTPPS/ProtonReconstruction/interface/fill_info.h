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

extern FillInfoCollection fillInfoCollection;

//----------------------------------------------------------------------------------------------------

void InitFillInfoCollection();


#endif
