#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <deque>
#include "Word.h"

using std::vector;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::deque;

typedef unordered_set<WordPtr>::const_iterator ConstUnsIt;
typedef unordered_map<WordPtr, size_t, std::hash<WordPtr>, std::equal_to<WordPtr>>::const_iterator ConstUMapIt;

class Context
{
public:
	const static double alpha; // base of multiplier as distance from word in ctx (not needed in the original implementation)
	const static size_t window_size; // distance of words from left and right taken into ctx

	Context(const deque<WordPtr>& words);
	void surround_word(WordPtr word);
	size_t hash(bool update = false);
	size_t get_freq() const;
	void inc_freq();
	void expandContext(WordPtr word);
	bool operator<(const Context& ctx) const;
	ConstUnsIt words_begin() const;
	ConstUnsIt words_end() const;
	ConstUnsIt words_find(const WordPtr& wp) const;
	ConstUMapIt surr_begin() const;
	ConstUMapIt surr_end() const;

private:
	size_t freq;
	size_t hash_val;
	unordered_set<WordPtr, std::hash<WordPtr>, std::equal_to<WordPtr>> words;
	unordered_map<WordPtr, size_t, std::hash<WordPtr>, std::equal_to<WordPtr>> surrounded;
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
}

#endif //_CONTEXT_H