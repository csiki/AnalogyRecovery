#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <deque>

using std::vector;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::deque;

class Word;

class Context
{
public:
	const static double alpha; // base of multiplier as distance from word in ctx (not needed in the original implementation)
	const static size_t window_size; // distance of words from left and right taken into ctx
	string words;

	Context(deque<shared_ptr<Word>> words);
	size_t hash(bool update = false) const;
	size_t get_freq() const;
	bool words_find(const shared_ptr<Word>& wp) const;
	void inc_freq();

private:
	unsigned freq;
};

typedef shared_ptr<Context> CtxPtr;

namespace std
{
	template <>
	struct hash<CtxPtr>
	{
		size_t operator()(const CtxPtr& wp) const;
	};

	template <>
	struct equal_to<CtxPtr>
	{
		bool operator()(const CtxPtr& wp1, const CtxPtr& wp2) const;
	};

	template <>
	struct less<CtxPtr>
	{
		bool operator()(const CtxPtr& wp1, const CtxPtr& wp2) const;
	};
}

#endif //_CONTEXT_H