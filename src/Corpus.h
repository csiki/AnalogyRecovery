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
#include "Word.h"
#include "Context.h"

using std::tuple;
using std::vector;
using std::set;
using std::unordered_set;
using std::string;
using std::shared_ptr;

typedef shared_ptr<Word> WordPtr;
typedef shared_ptr<Context> CtxPtr;

namespace std {

template <> struct equal_to<WordPtr>
{
	bool operator()(const WordPtr& wp1, const WordPtr& wp2)
	{
		return wp1->word == wp2->word;
	}

	bool operator()(const CtxPtr& cp1, const CtxPtr& cp2)
	{
		return cp1->hash() == cp2->hash();
	}
};

}

class Corpus
{
public:
    vector<string> source;
    unordered_set<WordPtr, std::hash<WordPtr>, std::equal_to<WordPtr>> vocabulary;
    unordered_set<CtxPtr, std::hash<CtxPtr>, std::equal_to<CtxPtr>> contexts;
    
	Corpus() {}
    void generate_voc_and_ctx();
	static bool is_forbidden(const string& str);
	static bool try_form_well(const string& orig, string& res);
	static void init_forbidden();

private:
	static unordered_set<char> forbidden_chars;
	static set<string> forbidden_words;

    void gen_voc();
    void gen_ctx();
};

#endif //_CORPUS_H