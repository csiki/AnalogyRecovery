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
	static bool split_ctx_at_sentence;
    
	/// Returns a word that fits best (in corpus) to 'b' as 'a' fits to 'a_' (analogy).
	WordPtr analogy_3_cos_add(WordPtr a, WordPtr a_, WordPtr b);
	string analogy_3_cos_add(string a, string a_, string b);
	WordPtr analogy_3_cos_mul(WordPtr a, WordPtr a_, WordPtr b);
	string analogy_3_cos_mul(string a, string a_, string b);

	Corpus(bool is_source_preprocessed_ = false);
    void generate_voc_and_ctx();
	void calc_feature_vectors();
	static bool try_form_well(string orig, string& res, bool lowcost = true);
	static void init();
	void ser_voc_and_ctx(std::ostream& voc_out, std::ostream& ctx_out) const;
	void deser_voc_and_ctx(std::istream& voc_in, std::istream& ctx_in);

private:
	static unordered_set<char> forbidden_chars;
	static unordered_set<string> forbidden_words;
	static unordered_set<char> endofsentence_chars;
	static double analogy_eps;
	deque<WordPtr> ctx_hist;
	unsigned char read_print_state;
	bool is_source_preprocessed; // use low cpu cost try_form_well() if true

	WordPtr read_word(std::istream& stream);
	vector<CtxPtr> arrange_ctx(WordPtr curr_word);
	void print_read_info(float rate);
};

#endif //_CORPUS_H