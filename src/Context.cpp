
#include "Context.h"
#include "Word.h"

const size_t Context::window_size = 2; // 2 left, 2 right from the word
const double Context::alpha = 0.9; // word in n distance get a alpha^0.9 multiplier

size_t std::hash<CtxPtr>::operator()(const CtxPtr& cp) const
{
	std::hash<string> string_hash;
	return string_hash(cp->words);
}

bool std::equal_to<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	std::equal_to<string> e;
	return e(cp1->words, cp2->words);
}

bool std::less<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	std::less<string> l;
	return l(cp1->words, cp2->words);
}

Context::Context(deque<WordPtr> words_)
{
	std::sort(words_.begin(), words_.end());
	size_t words_len = 1;
	for (auto& w : words_)
		words_len += w->word.length();

	words.reserve(words_len);
	for (auto& wp : words_)
		words += wp->word;
	
	words.shrink_to_fit();
	hash(true); // update hash
}

size_t Context::get_freq() const
{
    return freq;
}

void Context::inc_freq()
{
	++freq;
}

size_t Context::hash(bool update) const
{
	size_t hash_val = 0;
	auto str_c = words.c_str();
	while (*str_c)
		  hash_val = hash_val << 1 ^ *str_c++;

    return hash_val;
}

bool Context::words_find(const WordPtr& wp) const
{
	return words.find(wp->word) != string::npos;
}