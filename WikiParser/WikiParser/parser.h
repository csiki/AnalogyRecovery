#ifndef _PARSER_H
#define _PARSER_H

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

typedef function<bool(ifstream&, string&)> FilterFun;

struct Parser
{
	unordered_map<string, string> source_fileio;
	list<FilterFun> filters;
	size_t wsize; // window size
	unordered_set<char> non_alpha;
	unordered_set<char> sentence_ending_chars;
	unordered_set<string> forbidden_line_starters;
	unordered_set<string> ignored_section_titles;
	size_t ign_sec_title_max_len;

	Parser();
	void parse();
	bool check_forbidden_line_starters(ifstream& fin, string& line);
	bool to_lower(ifstream& fin, string& line);
	bool ignore_nearend_section_titles(ifstream& fin, string& line);
	bool rm_non_alpha(ifstream& fin, string& line);
	void extract_sentences(const string& line, ofstream& fout);
};

#endif