
// TODO update uml

#include <exception>
#include <iostream>
#include <iterator>
#include "SparseVector.h"
#include "Corpus.h"
#include "Word.h"

using namespace std;

int main(int argc, char* argv[])
{
	Corpus::init();
	Corpus corpus(true);

	// add sources
	string path = "c:/wiki_corpus/";
	corpus.sources_path.push_back(path + "englishText_0_10000_parsed");
	corpus.sources_path.push_back(path + "englishText_1000000_1010000_parsed");
	corpus.sources_path.push_back(path + "englishText_100000_110000_parsed");
	corpus.sources_path.push_back(path + "englishText_10000_20000_parsed");
	corpus.sources_path.push_back(path + "englishText_1010000_1020000_parsed");
	corpus.sources_path.push_back(path + "englishText_1020000_1030000_parsed");
	corpus.sources_path.push_back(path + "englishText_1030000_1040000_parsed");
	corpus.sources_path.push_back(path + "englishText_1040000_1050000_parsed");
	corpus.sources_path.push_back(path + "englishText_1050000_1060000_parsed");
	corpus.sources_path.push_back(path + "englishText_1060000_1070000_parsed");
	corpus.sources_path.push_back(path + "englishText_1070000_1080000_parsed");
	corpus.sources_path.push_back(path + "englishText_1080000_1090000_parsed");
	corpus.sources_path.push_back(path + "englishText_1090000_1100000_parsed");
	corpus.sources_path.push_back(path + "englishText_1100000_1110000_parsed");
	corpus.sources_path.push_back(path + "englishText_110000_120000_parsed");
	corpus.sources_path.push_back(path + "englishText_1110000_1120000_parsed");
	corpus.sources_path.push_back(path + "englishText_1120000_1130000_parsed");
	corpus.sources_path.push_back(path + "englishText_1130000_1140000_parsed");
	corpus.sources_path.push_back(path + "englishText_1140000_1150000_parsed");
	corpus.sources_path.push_back(path + "englishText_1150000_1160000_parsed");
	corpus.sources_path.push_back(path + "englishText_1160000_1170000_parsed");
	corpus.sources_path.push_back(path + "englishText_1170000_1180000_parsed");
	corpus.sources_path.push_back(path + "englishText_1180000_1190000_parsed");
	corpus.sources_path.push_back(path + "englishText_1190000_1200000_parsed");
	corpus.sources_path.push_back(path + "englishText_1200000_1210000_parsed");
	corpus.sources_path.push_back(path + "englishText_120000_130000_parsed");
	corpus.sources_path.push_back(path + "englishText_1210000_1220000_parsed");
	corpus.sources_path.push_back(path + "englishText_1220000_1230000_parsed");
	corpus.sources_path.push_back(path + "englishText_1230000_1240000_parsed");
	corpus.sources_path.push_back(path + "englishText_1240000_1250000_parsed");
	corpus.sources_path.push_back(path + "englishText_1250000_1260000_parsed");
	corpus.sources_path.push_back(path + "englishText_1260000_1270000_parsed");
	corpus.sources_path.push_back(path + "englishText_1270000_1280000_parsed");
	corpus.sources_path.push_back(path + "englishText_1280000_1290000_parsed");
	corpus.sources_path.push_back(path + "englishText_1290000_1300000_parsed");
	corpus.sources_path.push_back(path + "englishText_1300000_1310000_parsed");
	corpus.sources_path.push_back(path + "englishText_130000_140000_parsed");
	corpus.sources_path.push_back(path + "englishText_1310000_1320000_parsed");
	corpus.sources_path.push_back(path + "englishText_1320000_1330000_parsed");
	corpus.sources_path.push_back(path + "englishText_1330000_1340000_parsed");
	corpus.sources_path.push_back(path + "englishText_1340000_1350000_parsed");
	corpus.sources_path.push_back(path + "englishText_1350000_1360000_parsed");
	corpus.sources_path.push_back(path + "englishText_1360000_1370000_parsed");
	corpus.sources_path.push_back(path + "englishText_1370000_1380000_parsed");
	corpus.sources_path.push_back(path + "englishText_1380000_1390000_parsed");
	corpus.sources_path.push_back(path + "englishText_1390000_1400000_parsed");
	corpus.sources_path.push_back(path + "englishText_1400000_1410000_parsed");
	corpus.sources_path.push_back(path + "englishText_140000_150000_parsed");
	corpus.sources_path.push_back(path + "englishText_1410000_1420000_parsed");
	corpus.sources_path.push_back(path + "englishText_1420000_1430000_parsed");
	corpus.sources_path.push_back(path + "englishText_1430000_1440000_parsed");
	corpus.sources_path.push_back(path + "englishText_1440000_1450000_parsed");
	corpus.sources_path.push_back(path + "englishText_1450000_1460000_parsed");
	corpus.sources_path.push_back(path + "englishText_1460000_1470000_parsed");
	corpus.sources_path.push_back(path + "englishText_1470000_1480000_parsed");
	corpus.sources_path.push_back(path + "englishText_1480000_1490000_parsed");
	corpus.sources_path.push_back(path + "englishText_1490000_1500000_parsed");
	corpus.sources_path.push_back(path + "englishText_1500000_1510000_parsed");
	corpus.sources_path.push_back(path + "englishText_150000_160000_parsed");
	corpus.sources_path.push_back(path + "englishText_1510000_1520000_parsed");
	corpus.sources_path.push_back(path + "englishText_1520000_1530000_parsed");
	corpus.sources_path.push_back(path + "englishText_1530000_1540000_parsed");
	corpus.sources_path.push_back(path + "englishText_1540000_1550000_parsed");
	corpus.sources_path.push_back(path + "englishText_1550000_1560000_parsed");
	corpus.sources_path.push_back(path + "englishText_1560000_1570000_parsed");
	corpus.sources_path.push_back(path + "englishText_1570000_1580000_parsed");
	corpus.sources_path.push_back(path + "englishText_1580000_1590000_parsed");
	corpus.sources_path.push_back(path + "englishText_1590000_1600000_parsed");
	corpus.sources_path.push_back(path + "englishText_1600000_1610000_parsed");
	corpus.sources_path.push_back(path + "englishText_160000_170000_parsed");
	corpus.sources_path.push_back(path + "englishText_1610000_1620000_parsed");
	corpus.sources_path.push_back(path + "englishText_1620000_1630000_parsed");
	corpus.sources_path.push_back(path + "englishText_1630000_1640000_parsed");
	corpus.sources_path.push_back(path + "englishText_170000_180000_parsed");
	corpus.sources_path.push_back(path + "englishText_180000_190000_parsed");
	corpus.sources_path.push_back(path + "englishText_190000_200000_parsed");
	corpus.sources_path.push_back(path + "englishText_200000_210000_parsed");
	corpus.sources_path.push_back(path + "englishText_20000_30000_parsed");
	corpus.sources_path.push_back(path + "englishText_210000_220000_parsed");
	corpus.sources_path.push_back(path + "englishText_220000_230000_parsed");
	corpus.sources_path.push_back(path + "englishText_230000_240000_parsed");
	corpus.sources_path.push_back(path + "englishText_240000_250000_parsed");
	corpus.sources_path.push_back(path + "englishText_250000_260000_parsed");
	corpus.sources_path.push_back(path + "englishText_260000_270000_parsed");
	corpus.sources_path.push_back(path + "englishText_270000_280000_parsed");
	corpus.sources_path.push_back(path + "englishText_280000_290000_parsed");
	corpus.sources_path.push_back(path + "englishText_290000_300000_parsed");
	corpus.sources_path.push_back(path + "englishText_300000_310000_parsed");
	corpus.sources_path.push_back(path + "englishText_30000_40000_parsed");
	corpus.sources_path.push_back(path + "englishText_310000_320000_parsed");
	corpus.sources_path.push_back(path + "englishText_320000_330000_parsed");
	corpus.sources_path.push_back(path + "englishText_330000_340000_parsed");
	corpus.sources_path.push_back(path + "englishText_340000_350000_parsed");
	corpus.sources_path.push_back(path + "englishText_350000_360000_parsed");
	corpus.sources_path.push_back(path + "englishText_360000_370000_parsed");
	corpus.sources_path.push_back(path + "englishText_370000_380000_parsed");
	corpus.sources_path.push_back(path + "englishText_380000_390000_parsed");
	corpus.sources_path.push_back(path + "englishText_390000_400000_parsed");
	corpus.sources_path.push_back(path + "englishText_400000_410000_parsed");
	corpus.sources_path.push_back(path + "englishText_40000_50000_parsed");
	corpus.sources_path.push_back(path + "englishText_410000_420000_parsed");
	corpus.sources_path.push_back(path + "englishText_420000_430000_parsed");
	corpus.sources_path.push_back(path + "englishText_430000_440000_parsed");
	corpus.sources_path.push_back(path + "englishText_440000_450000_parsed");
	corpus.sources_path.push_back(path + "englishText_450000_460000_parsed");
	corpus.sources_path.push_back(path + "englishText_460000_470000_parsed");
	corpus.sources_path.push_back(path + "englishText_470000_480000_parsed");
	corpus.sources_path.push_back(path + "englishText_480000_490000_parsed");
	corpus.sources_path.push_back(path + "englishText_490000_500000_parsed");
	corpus.sources_path.push_back(path + "englishText_500000_510000_parsed");
	corpus.sources_path.push_back(path + "englishText_50000_60000_parsed");
	corpus.sources_path.push_back(path + "englishText_510000_520000_parsed");
	corpus.sources_path.push_back(path + "englishText_520000_530000_parsed");
	corpus.sources_path.push_back(path + "englishText_530000_540000_parsed");
	corpus.sources_path.push_back(path + "englishText_540000_550000_parsed");
	corpus.sources_path.push_back(path + "englishText_550000_560000_parsed");
	corpus.sources_path.push_back(path + "englishText_560000_570000_parsed");
	corpus.sources_path.push_back(path + "englishText_570000_580000_parsed");
	corpus.sources_path.push_back(path + "englishText_580000_590000_parsed");
	corpus.sources_path.push_back(path + "englishText_590000_600000_parsed");
	corpus.sources_path.push_back(path + "englishText_600000_610000_parsed");
	corpus.sources_path.push_back(path + "englishText_60000_70000_parsed");
	corpus.sources_path.push_back(path + "englishText_610000_620000_parsed");
	corpus.sources_path.push_back(path + "englishText_620000_630000_parsed");
	corpus.sources_path.push_back(path + "englishText_630000_640000_parsed");
	corpus.sources_path.push_back(path + "englishText_640000_650000_parsed");
	corpus.sources_path.push_back(path + "englishText_650000_660000_parsed");
	corpus.sources_path.push_back(path + "englishText_660000_670000_parsed");
	corpus.sources_path.push_back(path + "englishText_670000_680000_parsed");
	corpus.sources_path.push_back(path + "englishText_680000_690000_parsed");
	corpus.sources_path.push_back(path + "englishText_690000_700000_parsed");
	corpus.sources_path.push_back(path + "englishText_700000_710000_parsed");
	corpus.sources_path.push_back(path + "englishText_70000_80000_parsed");
	corpus.sources_path.push_back(path + "englishText_710000_720000_parsed");
	corpus.sources_path.push_back(path + "englishText_720000_730000_parsed");
	corpus.sources_path.push_back(path + "englishText_730000_740000_parsed");
	corpus.sources_path.push_back(path + "englishText_740000_750000_parsed");
	corpus.sources_path.push_back(path + "englishText_750000_760000_parsed");
	corpus.sources_path.push_back(path + "englishText_760000_770000_parsed");
	corpus.sources_path.push_back(path + "englishText_770000_780000_parsed");
	corpus.sources_path.push_back(path + "englishText_780000_790000_parsed");
	corpus.sources_path.push_back(path + "englishText_790000_800000_parsed");
	corpus.sources_path.push_back(path + "englishText_800000_810000_parsed");
	corpus.sources_path.push_back(path + "englishText_80000_90000_parsed");
	corpus.sources_path.push_back(path + "englishText_810000_820000_parsed");
	corpus.sources_path.push_back(path + "englishText_820000_830000_parsed");
	corpus.sources_path.push_back(path + "englishText_830000_840000_parsed");
	corpus.sources_path.push_back(path + "englishText_840000_850000_parsed");
	corpus.sources_path.push_back(path + "englishText_850000_860000_parsed");
	corpus.sources_path.push_back(path + "englishText_860000_870000_parsed");
	corpus.sources_path.push_back(path + "englishText_870000_880000_parsed");
	corpus.sources_path.push_back(path + "englishText_880000_890000_parsed");
	corpus.sources_path.push_back(path + "englishText_890000_900000_parsed");
	corpus.sources_path.push_back(path + "englishText_900000_910000_parsed");
	corpus.sources_path.push_back(path + "englishText_90000_100000_parsed");
	corpus.sources_path.push_back(path + "englishText_910000_920000_parsed");
	corpus.sources_path.push_back(path + "englishText_920000_930000_parsed");
	corpus.sources_path.push_back(path + "englishText_930000_940000_parsed");
	corpus.sources_path.push_back(path + "englishText_940000_950000_parsed");
	corpus.sources_path.push_back(path + "englishText_950000_960000_parsed");
	corpus.sources_path.push_back(path + "englishText_960000_970000_parsed");
	corpus.sources_path.push_back(path + "englishText_970000_980000_parsed");
	corpus.sources_path.push_back(path + "englishText_980000_990000_parsed");
	corpus.sources_path.push_back(path + "englishText_990000_1000000_parsed");

	corpus.generate_voc_and_ctx();
	corpus.calc_feature_vectors();

	cout << "rudi" << endl;

	return 0;
}