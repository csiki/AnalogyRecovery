#ifndef _CORPUS_H
#define _CORPUS_H

#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <memory>
#include <tuple>
#include <exception>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>
#include <string>
#include <deque>
#include <iostream>
#include <iomanip>
#include "Word.h"
#include "Context.h"

using std::tuple;
using std::vector;
using std::set;
using std::unordered_set;
using std::string;
using std::shared_ptr;
using std::deque;

class Corpus
{
public:
    vector<string> sources_path;
    unordered_set<WordPtr, std::hash<WordPtr>, std::equal_to<WordPtr>> vocabulary;
    unordered_set<CtxPtr, std::hash<CtxPtr>, std::equal_to<CtxPtr>> contexts;
    
	Corpus();
    void generate_voc_and_ctx();
	void calc_feature_vectors();
	static bool is_forbidden(const string& str);
	static bool try_form_well(const string& orig, string& res);
	static void init_forbidden();

private:
	static unordered_set<char> forbidden_chars;
	static set<string> forbidden_words;
	deque<WordPtr> ctx_hist;
	unsigned char read_print_state;

	WordPtr read_word(std::istream& stream);
	CtxPtr arrange_ctx(WordPtr curr_word);
	void print_read_info(float rate);
};

#endif //_CORPUS_H