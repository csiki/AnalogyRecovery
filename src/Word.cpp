
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

bool std::less<WordPtr>::operator()(const WordPtr& wp1, const WordPtr& wp2) const
{
	std::less<string> l;
	return l(wp1->word, wp2->word);
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

void Word::appears_in(const shared_ptr<Context> ctx)
{
	auto cmit = context_occurence.insert(std::make_pair(ctx, 1));
	if (!cmit.second)
		(*cmit.first).second++;
}

CtxOccMapConstIt Word::ctxocc_begin() const
{
	return context_occurence.begin();
}

CtxOccMapConstIt Word::ctxocc_end() const
{
	return context_occurence.end();
}

CtxOccMapConstIt Word::ctxocc_find(const CtxPtr& ctx) const
{
	return context_occurence.find(ctx);
}