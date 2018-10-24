#include "RecoCTPPS/ProtonReconstruction/interface/fill_info.h"

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
    // TODO
	fillInfoCollection.push_back(FillInfo(0, false, 297046, 302663, "2017_preTS2/preliminary"));
    /*
    fillInfoCollection.push_back(FillInfo(5839, false, 297046, 297050, "fill 5839"));
    fillInfoCollection.push_back(FillInfo(5840, false, 297056, 297057, "fill 5840"));
    fillInfoCollection.push_back(FillInfo(5842, false, 297099, 297101, "fill 5842"));
    fillInfoCollection.push_back(FillInfo(5845, false, 297113, 297114, "fill 5845"));
    fillInfoCollection.push_back(FillInfo(5848, false, 297168, 297182, "fill 5848"));
    fillInfoCollection.push_back(FillInfo(5849, false, 297211, 297229, "fill 5849"));
    fillInfoCollection.push_back(FillInfo(5856, false, 297281, 297316, "fill 5856"));
    fillInfoCollection.push_back(FillInfo(5862, false, 297359, 297359, "fill 5862"));
    fillInfoCollection.push_back(FillInfo(5864, false, 297411, 297411, "fill 5864"));
    fillInfoCollection.push_back(FillInfo(5865, false, 297424, 297435, "fill 5865"));
    fillInfoCollection.push_back(FillInfo(5868, false, 297467, 297469, "fill 5868"));
    fillInfoCollection.push_back(FillInfo(5870, false, 297474, 297474, "fill 5870"));
    fillInfoCollection.push_back(FillInfo(5872, false, 297483, 297488, "fill 5872"));
    fillInfoCollection.push_back(FillInfo(5873, false, 297494, 297505, "fill 5873"));
    fillInfoCollection.push_back(FillInfo(5876, false, 297557, 297563, "fill 5876"));
    fillInfoCollection.push_back(FillInfo(5878, false, 297598, 297606, "fill 5878"));
    fillInfoCollection.push_back(FillInfo(5880, false, 297620, 297620, "fill 5880"));
    fillInfoCollection.push_back(FillInfo(5882, false, 297656, 297666, "fill 5882"));
    fillInfoCollection.push_back(FillInfo(5883, false, 297670, 297678, "fill 5883"));
    fillInfoCollection.push_back(FillInfo(5885, false, 297713, 297715, "fill 5885"));
    fillInfoCollection.push_back(FillInfo(5887, false, 297722, 297723, "fill 5887"));
    fillInfoCollection.push_back(FillInfo(5919, false, 298653, 298653, "fill 5919"));
    fillInfoCollection.push_back(FillInfo(5920, false, 298673, 298681, "fill 5920"));
    fillInfoCollection.push_back(FillInfo(5930, false, 298809, 298809, "fill 5930"));
    fillInfoCollection.push_back(FillInfo(5934, false, 298853, 298855, "fill 5934"));
    fillInfoCollection.push_back(FillInfo(5942, false, 298996, 299000, "fill 5942"));
    fillInfoCollection.push_back(FillInfo(5946, false, 299042, 299042, "fill 5946"));
    fillInfoCollection.push_back(FillInfo(5950, false, 299061, 299067, "fill 5950"));
    fillInfoCollection.push_back(FillInfo(5952, false, 299096, 299096, "fill 5952"));
    fillInfoCollection.push_back(FillInfo(5954, false, 299149, 299149, "fill 5954"));
    fillInfoCollection.push_back(FillInfo(5958, false, 299178, 299185, "fill 5958"));
    fillInfoCollection.push_back(FillInfo(5960, false, 299316, 299329, "fill 5960"));
    fillInfoCollection.push_back(FillInfo(5962, false, 299368, 299370, "fill 5962"));
    fillInfoCollection.push_back(FillInfo(5963, false, 299380, 299381, "fill 5963"));
    fillInfoCollection.push_back(FillInfo(5965, false, 299393, 299396, "fill 5965"));
    fillInfoCollection.push_back(FillInfo(5966, false, 299420, 299420, "fill 5966"));
    fillInfoCollection.push_back(FillInfo(5971, false, 299443, 299443, "fill 5971"));
    fillInfoCollection.push_back(FillInfo(5974, false, 299450, 299450, "fill 5974"));
    fillInfoCollection.push_back(FillInfo(5976, false, 299477, 299481, "fill 5976"));
    fillInfoCollection.push_back(FillInfo(5979, false, 299592, 299598, "fill 5979"));
    fillInfoCollection.push_back(FillInfo(5980, false, 299614, 299617, "fill 5980"));
    fillInfoCollection.push_back(FillInfo(5984, false, 299649, 299650, "fill 5984"));
    fillInfoCollection.push_back(FillInfo(6012, false, 299996, 299996, "fill 6012"));
    fillInfoCollection.push_back(FillInfo(6015, false, 300007, 300007, "fill 6015"));
    fillInfoCollection.push_back(FillInfo(6016, false, 300015, 300050, "fill 6016"));
    fillInfoCollection.push_back(FillInfo(6018, false, 300079, 300079, "fill 6018"));
    fillInfoCollection.push_back(FillInfo(6019, false, 300087, 300088, "fill 6019"));
    fillInfoCollection.push_back(FillInfo(6020, false, 300104, 300107, "fill 6020"));
    fillInfoCollection.push_back(FillInfo(6021, false, 300117, 300117, "fill 6021"));
    fillInfoCollection.push_back(FillInfo(6024, false, 300122, 300124, "fill 6024"));
    fillInfoCollection.push_back(FillInfo(6026, false, 300155, 300157, "fill 6026"));
    fillInfoCollection.push_back(FillInfo(6030, false, 300226, 300226, "fill 6030"));
    fillInfoCollection.push_back(FillInfo(6031, false, 300233, 300241, "fill 6031"));
    fillInfoCollection.push_back(FillInfo(6035, false, 300280, 300284, "fill 6035"));
    fillInfoCollection.push_back(FillInfo(6041, false, 300364, 300375, "fill 6041"));
    fillInfoCollection.push_back(FillInfo(6044, false, 300389, 300401, "fill 6044"));
    fillInfoCollection.push_back(FillInfo(6046, false, 300459, 300467, "fill 6046"));
    fillInfoCollection.push_back(FillInfo(6048, false, 300497, 300500, "fill 6048"));
    fillInfoCollection.push_back(FillInfo(6050, false, 300514, 300517, "fill 6050"));
    fillInfoCollection.push_back(FillInfo(6052, false, 300538, 300561, "fill 6052"));
    fillInfoCollection.push_back(FillInfo(6053, false, 300574, 300576, "fill 6053"));
    fillInfoCollection.push_back(FillInfo(6054, false, 300631, 300636, "fill 6054"));
    fillInfoCollection.push_back(FillInfo(6055, false, 300673, 300676, "fill 6055"));
    fillInfoCollection.push_back(FillInfo(6057, false, 300742, 300742, "fill 6057"));
    fillInfoCollection.push_back(FillInfo(6060, false, 300777, 300785, "fill 6060"));
    fillInfoCollection.push_back(FillInfo(6061, false, 300806, 300817, "fill 6061"));
    fillInfoCollection.push_back(FillInfo(6072, false, 301046, 301046, "fill 6072"));
    fillInfoCollection.push_back(FillInfo(6079, false, 301086, 301088, "fill 6079"));
    fillInfoCollection.push_back(FillInfo(6082, false, 301141, 301142, "fill 6082"));
    fillInfoCollection.push_back(FillInfo(6084, false, 301161, 301165, "fill 6084"));
    fillInfoCollection.push_back(FillInfo(6086, false, 301179, 301183, "fill 6086"));
    fillInfoCollection.push_back(FillInfo(6089, false, 301281, 301283, "fill 6089"));
    fillInfoCollection.push_back(FillInfo(6090, false, 301298, 301298, "fill 6090"));
    fillInfoCollection.push_back(FillInfo(6091, false, 301323, 301323, "fill 6091"));
    fillInfoCollection.push_back(FillInfo(6093, false, 301330, 301330, "fill 6093"));
    fillInfoCollection.push_back(FillInfo(6094, false, 301359, 301359, "fill 6094"));
    fillInfoCollection.push_back(FillInfo(6096, false, 301383, 301384, "fill 6096"));
    fillInfoCollection.push_back(FillInfo(6097, false, 301391, 301399, "fill 6097"));
    fillInfoCollection.push_back(FillInfo(6098, false, 301417, 301417, "fill 6098"));
    fillInfoCollection.push_back(FillInfo(6104, false, 301447, 301450, "fill 6104"));
    fillInfoCollection.push_back(FillInfo(6105, false, 301461, 301461, "fill 6105"));
    fillInfoCollection.push_back(FillInfo(6106, false, 301472, 301480, "fill 6106"));
    fillInfoCollection.push_back(FillInfo(6110, false, 301519, 301532, "fill 6110"));
    fillInfoCollection.push_back(FillInfo(6114, false, 301567, 301567, "fill 6114"));
    fillInfoCollection.push_back(FillInfo(6116, false, 301627, 301627, "fill 6116"));
    fillInfoCollection.push_back(FillInfo(6119, false, 301664, 301665, "fill 6119"));
    fillInfoCollection.push_back(FillInfo(6123, false, 301694, 301694, "fill 6123"));
    fillInfoCollection.push_back(FillInfo(6136, false, 301912, 301914, "fill 6136"));
    fillInfoCollection.push_back(FillInfo(6138, false, 301941, 301941, "fill 6138"));
    fillInfoCollection.push_back(FillInfo(6140, false, 301959, 301960, "fill 6140"));
    fillInfoCollection.push_back(FillInfo(6141, false, 301969, 301970, "fill 6141"));
    fillInfoCollection.push_back(FillInfo(6142, false, 301984, 301987, "fill 6142"));
    fillInfoCollection.push_back(FillInfo(6143, false, 301997, 301998, "fill 6143"));
    fillInfoCollection.push_back(FillInfo(6146, false, 302019, 302019, "fill 6146"));
    fillInfoCollection.push_back(FillInfo(6147, false, 302026, 302044, "fill 6147"));
    fillInfoCollection.push_back(FillInfo(6152, false, 302131, 302132, "fill 6152"));
    fillInfoCollection.push_back(FillInfo(6155, false, 302159, 302159, "fill 6155"));
    fillInfoCollection.push_back(FillInfo(6156, false, 302163, 302166, "fill 6156"));
    fillInfoCollection.push_back(FillInfo(6158, false, 302225, 302225, "fill 6158"));
    fillInfoCollection.push_back(FillInfo(6159, false, 302228, 302229, "fill 6159"));
    fillInfoCollection.push_back(FillInfo(6160, false, 302239, 302240, "fill 6160"));
    fillInfoCollection.push_back(FillInfo(6161, false, 302262, 302263, "fill 6161"));
    fillInfoCollection.push_back(FillInfo(6165, false, 302277, 302280, "fill 6165"));
    fillInfoCollection.push_back(FillInfo(6167, false, 302322, 302322, "fill 6167"));
    fillInfoCollection.push_back(FillInfo(6168, false, 302328, 302328, "fill 6168"));
    fillInfoCollection.push_back(FillInfo(6169, false, 302337, 302350, "fill 6169"));
    fillInfoCollection.push_back(FillInfo(6170, false, 302388, 302388, "fill 6170"));
    fillInfoCollection.push_back(FillInfo(6171, false, 302392, 302393, "fill 6171"));
    fillInfoCollection.push_back(FillInfo(6174, false, 302448, 302448, "fill 6174"));
    fillInfoCollection.push_back(FillInfo(6175, false, 302472, 302476, "fill 6175"));
    fillInfoCollection.push_back(FillInfo(6176, false, 302479, 302479, "fill 6176"));
    fillInfoCollection.push_back(FillInfo(6177, false, 302484, 302494, "fill 6177"));
    fillInfoCollection.push_back(FillInfo(6179, false, 302509, 302509, "fill 6179"));
    fillInfoCollection.push_back(FillInfo(6180, false, 302513, 302513, "fill 6180"));
    fillInfoCollection.push_back(FillInfo(6182, false, 302522, 302526, "fill 6182"));
    fillInfoCollection.push_back(FillInfo(6185, false, 302548, 302555, "fill 6185"));
    fillInfoCollection.push_back(FillInfo(6186, false, 302563, 302573, "fill 6186"));
    fillInfoCollection.push_back(FillInfo(6189, false, 302596, 302597, "fill 6189"));
    fillInfoCollection.push_back(FillInfo(6191, false, 302634, 302635, "fill 6191"));
    fillInfoCollection.push_back(FillInfo(6192, false, 302646, 302654, "fill 6192"));
    fillInfoCollection.push_back(FillInfo(6193, false, 302660, 302663, "fill 6193"));
    */

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
