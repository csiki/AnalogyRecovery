
#include "Context.h"
#include "Word.h"

const size_t Context::window_size = 2; // 2 left, 2 right from the word
const double Context::alpha = 0.9; // word in n distance get a alpha^0.9 multiplier

size_t Context::get_freq() const
{
    return freq;
}

void Context::inc_freq()
{
	++freq;
}

void Context::expandContext(shared_ptr<Word> word)
{
	wow.push_back(word);
	hash_val = hash(*this); // update hash
}

bool Context::operator<(const Context& ctx) const
{
	return hash_val < ctx.hash_val;
}

size_t Context::hash(const Context& ctx)
{
	// concat words
	string str;
	for (auto w : ctx.wow)
		str += w->word;

	size_t h = 0;
	auto str_c = str.c_str();
	while (*str_c)
       h = h << 1 ^ *str_c++;

    return h;
}