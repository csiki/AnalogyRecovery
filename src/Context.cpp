
#include "Context.h"
#include "Word.h"

const size_t Context::window_size = 2; // 2 left, 2 right from the word
const double Context::alpha = 0.9; // word in n distance get a alpha^0.9 multiplier

size_t std::hash<CtxPtr>::operator()(const CtxPtr& cp) const
{
	std::hash<string> string_hash;
	size_t summed_hashes = 0;
	for (auto wit = cp->words_begin(); wit != cp->words_end(); ++wit)
		summed_hashes += string_hash((*wit)->word);
	
	std::hash<size_t> h;
	return h(summed_hashes);
}

bool std::equal_to<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	for (auto wit = cp1->words_begin(); wit != cp1->words_end(); ++wit)
		if (cp2->words_find(*wit) == cp2->words_end())
			return false;
	return true;
}

Context::Context(const deque<WordPtr>& words_)
{
	words.insert(words_.begin(), words_.end());
	hash(true); // update hash
}

void Context::surround_word(WordPtr word)
{
	auto insw = surrounded.insert(std::make_pair(word, 1));
	if (!insw.second)
		(*insw.first).second += 1; // not inserted, thus already inside
}

size_t Context::get_freq() const
{
    return freq;
}

void Context::inc_freq()
{
	++freq;
}

void Context::expandContext(WordPtr word)
{
	words.insert(word);
	hash_val = hash(true); // update hash
}

bool Context::operator<(const Context& ctx) const
{
	return hash_val < ctx.hash_val;
}

size_t Context::hash(bool update)
{
	if (update || hash_val == 0)
	{
		// concat words
		string str;
		for (auto w : words)
			str += w->word;
		// calc hash
		hash_val = 0;
		auto str_c = str.c_str();
		while (*str_c)
		   hash_val = hash_val << 1 ^ *str_c++;
	}

    return hash_val;
}

ConstUnsIt Context::words_begin() const
{
	return words.begin();
}

ConstUnsIt Context::words_end() const
{
	return words.end();
}

ConstUnsIt Context::words_find(const WordPtr& wp) const
{
	return words.find(wp);
}

ConstUMapIt Context::surr_begin() const
{
	return surrounded.begin();
}

ConstUMapIt Context::surr_end() const
{
	return surrounded.end();
}