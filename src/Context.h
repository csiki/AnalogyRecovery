#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::string;

class Word;

class Context
{
public:
	const static double alpha; // base of multiplier as distance from word in ctx
	const static size_t window_size; // distance of words from left and right taken into ctx

	static size_t hash(const Context& ctx);
	size_t get_freq() const;
	void inc_freq();
	void expandContext(shared_ptr<Word> word);
	bool operator<(const Context& ctx) const;

private:
	size_t freq;
	vector<shared_ptr<Word>> wow;
	size_t hash_val;
};

#endif //_CONTEXT_H