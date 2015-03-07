
#include "Context.h"
#include "Word.h"

const size_t Context::window_size = 2; // 2 left, 2 right from the word
const double Context::alpha = 0.9; // word in n distance get a alpha^0.9 multiplier

size_t std::hash<CtxPtr>::operator()(const CtxPtr& cp) const
{
	std::hash<size_t> h;
	return h(cp->hash());
}

bool std::equal_to<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	return std::equal(cp1->words_begin(), cp1->words_end(), cp2->words_begin(),
		[] (const WordPtr& wp1, const WordPtr& wp2) {
			return wp1->word == wp2->word;
	});
}

Context::Context(const deque<shared_ptr<Word>>& words_)
{
	words = words_;
	hash(true);
}

void Context::surround_word(shared_ptr<Word> word)
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
	words.push_back(word);
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

ConstDeqIt Context::words_begin() const
{
	return words.begin();
}

ConstDeqIt Context::words_end() const
{
	return words.end();
}

ConstUMapIt Context::surr_begin() const
{
	return surrounded.begin();
}

ConstUMapIt Context::surr_end() const
{
	return surrounded.end();
}