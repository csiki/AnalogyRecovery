#ifndef _WORD_H
#define _WORD_H

#include <unordered_map>
#include <memory>
#include <cmath>
#include <string>
#include "SparseVector.h"
#include "Context.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::pair;

typedef map<shared_ptr<Context>, unsigned>::const_iterator CtxOccMapConstIt;

class Word
{
public:
	const string word;
    SparseVector<float> features;
	
	Word(string word);
	static Word null_word();
	static bool is_null_word(const Word& word);
    size_t get_freq() const;
    void inc_freq();
	void appears_in(const shared_ptr<Context> ctx);
	CtxOccMapConstIt ctxocc_begin() const;
	CtxOccMapConstIt ctxocc_end() const;
	CtxOccMapConstIt ctxocc_find(const CtxPtr& ctx) const;

private:
    unsigned freq;
	map<shared_ptr<Context>, unsigned> context_occurence;
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

	template <>
	struct less<WordPtr>
	{
		bool operator()(const WordPtr& wp1, const WordPtr& wp2) const;
	};
}

#endif //_WORD_H