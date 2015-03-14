
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
	std::less<string> e;
	return e(wp1->word, wp2->word);
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

bool Word::operator<(const Word& w) const
{
    return std::lexicographical_compare(word.begin(), word.end(), w.word.begin(), w.word.end());
}

std::ostream& operator<<(std::ostream& out, const WordPtr& wp)
{
	out << wp->word << " " << wp->get_freq() << std::endl;
	return out;
}

std::istream& operator>>(std::istream& in, WordPtr& wp)
{
	string word;
	size_t freq;
	in >> word >> freq;
	wp = std::make_shared<Word>(Word(word, freq));
	return in;
}