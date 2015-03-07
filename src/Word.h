#ifndef _WORD_H
#define _WORD_H

#include <unordered_map>
#include <memory>
#include <cmath>
#include <string>
#include "SparseVector.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::pair;

class Context;

class Word
{
public:
	const string word;
    SparseVector<double> features;
	
	Word(string word);
	static Word null_word();
	static bool is_null_word(const Word& word);
    size_t get_freq() const;
    void inc_freq();
    //void appears_in(shared_ptr<Context> ctx);
    //void calc_features(size_t corpus_size); // called after freq and ctx_freq are given // old
    bool operator<(const Word& w) const;

private:
    size_t freq;
    //unordered_map<shared_ptr<Context>, size_t, std::hash<CtxPtr>, std::equal_to<CtxPtr>> ctx_freq;

	Word() : word(""), freq(0) {} // create null Word
};

typedef shared_ptr<Word> WordPtr;

namespace std
{
	template <>
	struct hash<WordPtr>
	{
		size_t operator()(const WordPtr& wp) const;
	};

	template <>
	struct equal_to<WordPtr>
	{
		bool operator()(const WordPtr& wp1, const WordPtr& wp2) const;
	};
}

#endif //_WORD_H