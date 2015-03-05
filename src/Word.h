#ifndef _WORD_H
#define _WORD_H

#include <unordered_map>
#include <memory>
#include <cmath>
#include "SparseVector.h"
#include "Context.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
using std::pair;

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
    void appears_in(shared_ptr<Context> ctx, size_t distance);
    void calc_features(size_t corpus_size); // called after freq and ctx_freq are given
    bool operator<(const Word& w) const;

private:
    size_t freq;
    unordered_map<shared_ptr<Context>, double> ctx_freq;

	Word() : word(""), freq(0) {} // create null Word
};

#endif //_WORD_H