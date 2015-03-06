
#include "Word.h"
#include "Context.h"

size_t std::hash<WordPtr>::operator()(const WordPtr& wp) const
{
	std::hash<string> h;
	return h(wp->word);
}

bool std::equal_to<WordPtr>::operator()(const WordPtr& wp1, const WordPtr& wp2) const
{
	std::equal_to<string> e;
	return e(wp1->word, wp2->word);
}

Word::Word(string word_) : word(word_)
{
	freq = 1;
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

void Word::appears_in(shared_ptr<Context> ctx)
{
	auto insf = ctx_freq.insert(std::make_pair(ctx, 1));
	if (!insf.second)
		(*insf.first).second += 1; // not inserted, thus already inside
}

void Word::calc_features(size_t corpus_size)
{
	// TODO
}

bool Word::operator<(const Word& w) const
{
    return std::lexicographical_compare(word.begin(), word.end(), w.word.begin(), w.word.end());
}