
#include <iostream>
#include <vector>
#include <string>
#include "parser.h"

using namespace std;

/// extract sentences - exclude titles, infoboxes, captions, external links, see also part, xml tags
/// filter out nonalphanumeric tokens - numbers, forbidden chars
/// title: filtered as <n token sentence
/// to lowercase
/// xml tags: line starting with "<" is skipped
/// ext. links, see also: filter out everything between "External links" or "See also" or "Footnotes" or "Resources" or "Further reading" or "References", and ENDOFARTICLE
/// sentence: ends with +[a-zA-Z][.?!] not interrupted by line break --> first rm nonalpha words; every sentence to new line
/// params: sources, window size, forbidden nonalphanumeric tokens, sentence ending characters, forbbiden line starters, ignored before-end section titles
/// 1) read line, 2) skip if forbidden line starter, 3) to lower, 4) if ignored sec. title --> exclude till endofarticle, 5) rm non alpha words, 6) extract sentences with >n num of words splited by sentence ending chars
int main(int argc, char* argv[])
{
	string path = "c:/wiki_corpus/";

	Parser parser;
	parser.wsize = 2;

	// sources
	parser.source_fileio.insert(make_pair(path + "englishText_0_10000", path + "englishText_0_10000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1000000_1010000", path + "englishText_1000000_1010000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_100000_110000", path + "englishText_100000_110000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_10000_20000", path + "englishText_10000_20000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1010000_1020000", path + "englishText_1010000_1020000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1020000_1030000", path + "englishText_1020000_1030000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1030000_1040000", path + "englishText_1030000_1040000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1040000_1050000", path + "englishText_1040000_1050000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1050000_1060000", path + "englishText_1050000_1060000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1060000_1070000", path + "englishText_1060000_1070000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1070000_1080000", path + "englishText_1070000_1080000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1080000_1090000", path + "englishText_1080000_1090000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1090000_1100000", path + "englishText_1090000_1100000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1100000_1110000", path + "englishText_1100000_1110000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_110000_120000", path + "englishText_110000_120000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1110000_1120000", path + "englishText_1110000_1120000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1120000_1130000", path + "englishText_1120000_1130000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1130000_1140000", path + "englishText_1130000_1140000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1140000_1150000", path + "englishText_1140000_1150000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1150000_1160000", path + "englishText_1150000_1160000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1160000_1170000", path + "englishText_1160000_1170000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1170000_1180000", path + "englishText_1170000_1180000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1180000_1190000", path + "englishText_1180000_1190000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1190000_1200000", path + "englishText_1190000_1200000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1200000_1210000", path + "englishText_1200000_1210000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_120000_130000", path + "englishText_120000_130000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1210000_1220000", path + "englishText_1210000_1220000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1220000_1230000", path + "englishText_1220000_1230000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1230000_1240000", path + "englishText_1230000_1240000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1240000_1250000", path + "englishText_1240000_1250000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1250000_1260000", path + "englishText_1250000_1260000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1260000_1270000", path + "englishText_1260000_1270000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1270000_1280000", path + "englishText_1270000_1280000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1280000_1290000", path + "englishText_1280000_1290000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1290000_1300000", path + "englishText_1290000_1300000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1300000_1310000", path + "englishText_1300000_1310000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_130000_140000", path + "englishText_130000_140000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1310000_1320000", path + "englishText_1310000_1320000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1320000_1330000", path + "englishText_1320000_1330000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1330000_1340000", path + "englishText_1330000_1340000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1340000_1350000", path + "englishText_1340000_1350000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1350000_1360000", path + "englishText_1350000_1360000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1360000_1370000", path + "englishText_1360000_1370000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1370000_1380000", path + "englishText_1370000_1380000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1380000_1390000", path + "englishText_1380000_1390000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1390000_1400000", path + "englishText_1390000_1400000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1400000_1410000", path + "englishText_1400000_1410000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_140000_150000", path + "englishText_140000_150000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1410000_1420000", path + "englishText_1410000_1420000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1420000_1430000", path + "englishText_1420000_1430000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1430000_1440000", path + "englishText_1430000_1440000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1440000_1450000", path + "englishText_1440000_1450000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1450000_1460000", path + "englishText_1450000_1460000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1460000_1470000", path + "englishText_1460000_1470000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1470000_1480000", path + "englishText_1470000_1480000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1480000_1490000", path + "englishText_1480000_1490000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1490000_1500000", path + "englishText_1490000_1500000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1500000_1510000", path + "englishText_1500000_1510000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_150000_160000", path + "englishText_150000_160000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1510000_1520000", path + "englishText_1510000_1520000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1520000_1530000", path + "englishText_1520000_1530000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1530000_1540000", path + "englishText_1530000_1540000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1540000_1550000", path + "englishText_1540000_1550000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1550000_1560000", path + "englishText_1550000_1560000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1560000_1570000", path + "englishText_1560000_1570000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1570000_1580000", path + "englishText_1570000_1580000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1580000_1590000", path + "englishText_1580000_1590000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1590000_1600000", path + "englishText_1590000_1600000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1600000_1610000", path + "englishText_1600000_1610000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_160000_170000", path + "englishText_160000_170000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1610000_1620000", path + "englishText_1610000_1620000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1620000_1630000", path + "englishText_1620000_1630000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_1630000_1640000", path + "englishText_1630000_1640000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_170000_180000", path + "englishText_170000_180000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_180000_190000", path + "englishText_180000_190000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_190000_200000", path + "englishText_190000_200000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_200000_210000", path + "englishText_200000_210000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_20000_30000", path + "englishText_20000_30000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_210000_220000", path + "englishText_210000_220000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_220000_230000", path + "englishText_220000_230000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_230000_240000", path + "englishText_230000_240000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_240000_250000", path + "englishText_240000_250000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_250000_260000", path + "englishText_250000_260000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_260000_270000", path + "englishText_260000_270000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_270000_280000", path + "englishText_270000_280000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_280000_290000", path + "englishText_280000_290000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_290000_300000", path + "englishText_290000_300000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_300000_310000", path + "englishText_300000_310000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_30000_40000", path + "englishText_30000_40000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_310000_320000", path + "englishText_310000_320000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_320000_330000", path + "englishText_320000_330000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_330000_340000", path + "englishText_330000_340000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_340000_350000", path + "englishText_340000_350000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_350000_360000", path + "englishText_350000_360000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_360000_370000", path + "englishText_360000_370000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_370000_380000", path + "englishText_370000_380000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_380000_390000", path + "englishText_380000_390000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_390000_400000", path + "englishText_390000_400000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_400000_410000", path + "englishText_400000_410000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_40000_50000", path + "englishText_40000_50000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_410000_420000", path + "englishText_410000_420000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_420000_430000", path + "englishText_420000_430000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_430000_440000", path + "englishText_430000_440000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_440000_450000", path + "englishText_440000_450000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_450000_460000", path + "englishText_450000_460000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_460000_470000", path + "englishText_460000_470000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_470000_480000", path + "englishText_470000_480000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_480000_490000", path + "englishText_480000_490000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_490000_500000", path + "englishText_490000_500000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_500000_510000", path + "englishText_500000_510000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_50000_60000", path + "englishText_50000_60000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_510000_520000", path + "englishText_510000_520000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_520000_530000", path + "englishText_520000_530000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_530000_540000", path + "englishText_530000_540000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_540000_550000", path + "englishText_540000_550000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_550000_560000", path + "englishText_550000_560000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_560000_570000", path + "englishText_560000_570000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_570000_580000", path + "englishText_570000_580000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_580000_590000", path + "englishText_580000_590000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_590000_600000", path + "englishText_590000_600000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_600000_610000", path + "englishText_600000_610000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_60000_70000", path + "englishText_60000_70000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_610000_620000", path + "englishText_610000_620000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_620000_630000", path + "englishText_620000_630000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_630000_640000", path + "englishText_630000_640000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_640000_650000", path + "englishText_640000_650000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_650000_660000", path + "englishText_650000_660000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_660000_670000", path + "englishText_660000_670000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_670000_680000", path + "englishText_670000_680000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_680000_690000", path + "englishText_680000_690000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_690000_700000", path + "englishText_690000_700000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_700000_710000", path + "englishText_700000_710000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_70000_80000", path + "englishText_70000_80000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_710000_720000", path + "englishText_710000_720000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_720000_730000", path + "englishText_720000_730000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_730000_740000", path + "englishText_730000_740000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_740000_750000", path + "englishText_740000_750000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_750000_760000", path + "englishText_750000_760000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_760000_770000", path + "englishText_760000_770000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_770000_780000", path + "englishText_770000_780000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_780000_790000", path + "englishText_780000_790000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_790000_800000", path + "englishText_790000_800000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_800000_810000", path + "englishText_800000_810000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_80000_90000", path + "englishText_80000_90000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_810000_820000", path + "englishText_810000_820000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_820000_830000", path + "englishText_820000_830000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_830000_840000", path + "englishText_830000_840000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_840000_850000", path + "englishText_840000_850000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_850000_860000", path + "englishText_850000_860000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_860000_870000", path + "englishText_860000_870000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_870000_880000", path + "englishText_870000_880000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_880000_890000", path + "englishText_880000_890000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_890000_900000", path + "englishText_890000_900000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_900000_910000", path + "englishText_900000_910000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_90000_100000", path + "englishText_90000_100000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_910000_920000", path + "englishText_910000_920000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_920000_930000", path + "englishText_920000_930000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_930000_940000", path + "englishText_930000_940000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_940000_950000", path + "englishText_940000_950000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_950000_960000", path + "englishText_950000_960000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_960000_970000", path + "englishText_960000_970000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_970000_980000", path + "englishText_970000_980000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_980000_990000", path + "englishText_980000_990000_parsed"));
	parser.source_fileio.insert(make_pair(path + "englishText_990000_1000000", path + "englishText_990000_1000000_parsed"));
	
	// non-alpha
	parser.non_alpha.insert('<');
	parser.non_alpha.insert('>');
	parser.non_alpha.insert('=');
	parser.non_alpha.insert('|');
	parser.non_alpha.insert('/');
	parser.non_alpha.insert('\\');
	for (char n = '0'; n <= '9'; ++n)
		parser.non_alpha.insert(n);

	// sentence-ending
	parser.sentence_ending_chars.insert('.');
	parser.sentence_ending_chars.insert('?');
	parser.sentence_ending_chars.insert('!');

	// forbidden line starters
	parser.forbidden_line_starters.insert("<");
	parser.forbidden_line_starters.insert("ENDOFARTICLE");

	// ignored sec. titles without spaces
	parser.ignored_section_titles.insert("Externallinks.");
	parser.ignored_section_titles.insert("Seealso.");
	parser.ignored_section_titles.insert("Footnotes.");
	parser.ignored_section_titles.insert("Resources.");
	parser.ignored_section_titles.insert("Furtherreading.");
	parser.ignored_section_titles.insert("References.");

	parser.parse();

	return 0;
}