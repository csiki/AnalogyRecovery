
#include "Word.h"
#include "Context.h"

Word::Word(string word_) : word(word_)
{
	// TODO first SparseVector impl
}

Word Word::null_word()
{
	return Word();
}

bool Word::is_null_word(const Word& word)
{
	return word.word.empty();
}

size_t Word::get_freq() const
{
    return freq;
}

void Word::inc_freq()
{
	++freq;
}

void Word::appears_in(shared_ptr<Context> ctx, size_t distance)
{
	ctx_freq.insert(std::make_pair(ctx, std::pow(Context::alpha, distance)));
}

void Word::calc_features(size_t corpus_size)
{
	// TODO
}

bool Word::operator<(const Word& w) const
{
    return std::lexicographical_compare(word.begin(), word.end(), w.word.begin(), w.word.end());
}