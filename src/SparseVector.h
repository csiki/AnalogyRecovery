#ifndef _SPARSEVECTOR_H
#define _SPARSEVECTOR_H

#include <map>
#include <algorithm>
#include <iterator>
#include <vector>
#include <exception>
#include <numeric>
#include <set>

using std::map;
using std::pair;
using std::vector;
using std::set;

template <typename T>
class SparseVector
{
public:
	SparseVector(size_t len_ = 0, T defval_ = T()) : len(len_), defval(defval_) {}

	// ordinary operators
    SparseVector operator+(const SparseVector& sv) const
	{
		check_dimension_match(sv);

		SparseVector res = *this;
		for (auto& v : sv.values)
			res[v.first] += v.second;
		
		return res;
	}

    SparseVector operator-(const SparseVector& sv) const
	{
		check_dimension_match(sv);

		SparseVector res = *this;
		for (auto& v : sv.values)
			res[v.first] -= v.second;

		return res;
	}

    T operator*(const SparseVector& sv) const
	{
		check_dimension_match(sv);

		T res = defval;
		auto intersect = intersect_keys(sv);
		for (auto i : intersect)
			res += values[i] * sv.values[i];

		return res;
	}

    SparseVector operator*(const T& c) const
	{
		SparseVector res = *this;
		for (auto& v : values)
			v.second *= c;

		return res;
	}

    SparseVector operator/(const T& c) const
	{
		SparseVector res = *this;
		for (auto& v : values)
			v.second /= c;

		return res;
	}

	// move operators TODO
	SparseVector operator+(SparseVector&& sv) const
	{
		check_dimension_match(sv);

		for (auto& v : values)
			sv[v.first] += v.second;
		
		return sv;
	}

    SparseVector operator-(SparseVector&& sv) const
	{
		check_dimension_match(sv);

		auto un = union_keys(sv);
		for (auto& u : un)
			sv[u] = (*this)[u] - sv[u];
		
		return sv;
	}

    T operator*(SparseVector&& sv) const
	{
		// TODO
		return SparseVector();
	}

    SparseVector operator*(T&& c) const
	{
		// TODO
		return SparseVector();
	}

    SparseVector operator/(T&& c) const
	{
		// TODO
		return SparseVector();
	}

	T abs() const
	{
		return std::sqrt(
			std::for_each(values.begin(), values.end(),
				[] (const T& val) { square_sum += val * val; }));
	}

	size_t get_len() const
	{
		return len;
	}

	void set_len(size_t len_)
	{
		len = len_;
	}

	T& operator[](size_t index)
	{
		if (len <= index)
			throw std::exception("Index out of bounds!");
		
		if (values.find(index) == values.end())
			values[index] = defval;

		return values[index];
	}

	const T& operator[](size_t index) const // does not increase container size // TODO is this called when intended or the other operator[] ???
	{
		if (len <= index)
			throw std::exception("Index out of bounds!");
		
		if (values.find(index) == values.end())
			return defval;

		return values[index];
	}

	/*static bool values_less_than(const pair<size_t, T>& v1, const pair<size_t, T>& v2)
	{
		return v1.first < v2.first;
	}*/

private:
	T defval; // always additive identity ~ like 0
	size_t len;
    map<size_t, T> values;

	void check_dimension_match(const SparseVector& sv)
	{
		if (len != sv.len)
			throw std::exception("SparseVector dimension mismatch!");
	}

	inline set<size_t> intersect_keys(const map<size_t, T>& vs)
	{
		set<size_t> intersect;
		std::set_intersection(values.begin(), values.end(),
			vs.values.begin(), vs.values.end(),
			std::inserter(intersect), key_comp);

		return intersect;
	}

	inline set<size_t> difference_keys(const map<size_t, T>& vs)
	{
		set<size_t> diff;
		std::set_difference(values.begin(), values.end(),
			vs.values.begin(), vs.values.end(),
			std::inserter(diff), key_comp);

		return diff;
	}

	inline set<size_t> union_keys(const map<size_t, T>& vs)
	{
		set<size_t> un;
		std::set_union(values.begin(), values.end(),
			vs.values.begin(), vs.values.end(),
			std::inserter(un), key_comp);

		return un;
	}
};

#endif //_SPARSEVECTOR_H