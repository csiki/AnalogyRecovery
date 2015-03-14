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
    SparseVector<float> features;
	
	Word::Word(string word_, size_t freq_from = 1) : word(word_), freq(freq_from) {}
	static Word null_word();
	static bool is_null_word(const Word& word);
    size_t get_freq() const;
    void inc_freq();
    bool operator<(const Word& w) const;

private:
    unsigned freq;

	Word() : word(""), freq(0) {} // create null Word
};

typedef shared_ptr<Word> WordPtr;

std::ostream& operator<<(std::ostream& out, const WordPtr& wp);
std::istream& operator>>(std::istream& in, WordPtr& wp);

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