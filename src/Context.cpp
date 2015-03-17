
#include "Context.h"
#include "Word.h"

const unsigned char Context::window_size = 2; // 2 left, 2 right from the word
const double Context::distance_multiplier = 1.0; // word in n distance get a alpha^0.9 multiplier

size_t std::hash<CtxPtr>::operator()(const CtxPtr& cp) const
{
	std::hash<string> h;
	return h(cp->ctx->word) + cp->pos;
}

bool std::equal_to<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	return cp1->pos == cp2->pos && cp1->ctx == cp2->ctx;
}

bool std::less<CtxPtr>::operator()(const CtxPtr& cp1, const CtxPtr& cp2) const
{
	return cp1->ctx < cp2->ctx || (cp1->ctx == cp2->ctx && cp1->pos < cp2->pos);
}

Context::Context(WordPtr ctx_, int pos_, size_t freq_, SurrMap surrounded_) : ctx(ctx_), pos(pos_), freq(freq_), surrounded(surrounded_) {}

void Context::surround_word(WordPtr word, size_t freq_from)
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

ConstUMapIt Context::surr_begin() const
{
	return surrounded.begin();
}

ConstUMapIt Context::surr_end() const
{
	return surrounded.end();
}

void Context::update_surr(const unordered_set<WordPtr, std::hash<WordPtr>, std::equal_to<WordPtr>>& vocabulary)
{
	SurrMap tmp_surr;
	for (auto& s : surrounded)
	{
		auto real_wp = vocabulary.find(s.first);
		if (real_wp == vocabulary.end())
			throw std::exception("Word in context cannot be found in the vocabulary!");
		tmp_surr.insert(std::make_pair(*real_wp, s.second));
	}
	surrounded = tmp_surr;
}

std::ostream& operator<<(std::ostream& out, const CtxPtr& cp)
{
	out << cp->ctx->word << " " << cp->get_freq() << " " << (int)cp->pos << " ";
	for (auto& s = cp->surr_begin(); s != cp->surr_end(); ++s)
	{
		out << s->first->word << " " << s->second << " ";
	}
	out << std::endl;
	return out;
}

std::istream& operator>>(std::istream& in, CtxPtr& wp)
{
	string line;
	std::getline(in, line, '\n');
	std::istringstream ssline(line);

	string ctx_s;
	WordPtr ctx;
	size_t freq;
	int pos;
	SurrMap surrounded;

	ssline >> ctx_s >> freq >> pos;

	string word;
	size_t wfreq;
	while (!ssline.eof())
	{
		ssline >> word >> wfreq;
		word.shrink_to_fit();
		surrounded.insert(std::make_pair(std::make_shared<Word>(Word(word, 0)), wfreq));
	}

	wp = unique_ptr<Context>(new Context(std::make_shared<Word>(ctx_s), pos, freq, surrounded));
	return in;
}