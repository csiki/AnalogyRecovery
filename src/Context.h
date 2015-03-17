#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "Word.h"

using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::unordered_set;
using std::deque;
using std::string;
using std::map;

typedef map<WordPtr, unsigned, std::less<WordPtr>> SurrMap;
typedef unordered_set<WordPtr>::const_iterator ConstUnsIt;
typedef SurrMap::const_iterator ConstUMapIt;

class Context
{
public:
	const static double distance_multiplier; // base of multiplier as distance from word in ctx (not needed in the original implementation)
	const static unsigned char window_size; // distance of words from left and right taken into ctx
	const int pos; // distance from the word in this context
	const WordPtr ctx; // the one word that forms the context

	Context(WordPtr ctx_, int pos_, size_t freq_ = 1, SurrMap surrounded_ = SurrMap());
	void surround_word(WordPtr word, size_t freq_from = 1);
	size_t get_freq() const;
	void inc_freq();
	ConstUMapIt surr_begin() const;
	ConstUMapIt surr_end() const;
	void update_surr(const unordered_set<WordPtr, std::hash<WordPtr>, std::equal_to<WordPtr>>& vocabulary);

private:
	unsigned freq;
	SurrMap surrounded;
};

typedef unique_ptr<Context> CtxPtr; // FIXME to unique_ptr

std::ostream& operator<<(std::ostream& out, const CtxPtr& wp);
std::istream& operator>>(std::istream& in, CtxPtr& wp);

namespace std
{
	template <>
	struct hash<CtxPtr>
	{
		size_t operator()(const CtxPtr& cp) const;
	};

	template <>
	struct equal_to<CtxPtr>
	{
		bool operator()(const CtxPtr& cp1, const CtxPtr& cp2) const;
	};

	template <>
	struct less<CtxPtr>
	{
		bool operator()(const CtxPtr& cp1, const CtxPtr& cp2) const;
	};
}

#endif //_CONTEXT_H