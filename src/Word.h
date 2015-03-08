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
    bool operator<(const Word& w) const;

private:
    size_t freq;

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