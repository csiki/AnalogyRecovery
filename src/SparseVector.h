#ifndef _SPARSEVECTOR_H
#define _SPARSEVECTOR_H

#include <map>
#include <algorithm>
#include <iterator>
#include <vector>
#include <exception>
#include <numeric>
#include <set>
#include <iostream>

using std::map;
using std::pair;
using std::vector;
using std::set;

template <typename K, typename V>
set<K> get_map_keys(typename map<K, V>::const_iterator& first, typename map<K, V>::const_iterator& last)
{
	set<K> res;
	std::transform(first, last, std::inserter(res, res.begin()),
		[] (const pair<K, V>& p) { return p.first; });
	return res;
}

template <typename T>
class SparseVector
{
public:
	SparseVector(size_t len_ = 0) : len(len_) {}

	// analogy operators
	static T cos_sim(const SparseVector& sv1, const SparseVector& sv2)
	{
		return (sv1 * sv2) / (sv1.abs() * sv2.abs());
	}
	static T cos_sim(SparseVector&& sv1, const SparseVector& sv2)
	{
		return (sv1 * sv2) / (sv1.abs() * sv2.abs());
	}
	static T cos_sim(const SparseVector& sv1, SparseVector&& sv2)
	{
		return (sv1 * sv2) / (sv1.abs() * sv2.abs());
	}
	static T cos_sim(SparseVector&& sv1, SparseVector&& sv2)
	{
		return (sv1 * sv2) / (sv1.abs() * sv2.abs());
	}

	// ordinary operators
    SparseVector operator+(const SparseVector& sv) const
	{
		check_compatibility(sv);

		auto res = *this;
		for (auto& v : sv.values)
			res[v.first] += v.second;

		res.clear_def_values();
		
		return res;
	}

    SparseVector operator-(const SparseVector& sv) const
	{
		check_compatibility(sv);

		SparseVector res = *this;
		for (auto& v : sv.values)
			res.values[v.first] -= v.second;

		res.clear_def_values();

		return res;
	}

    T operator*(const SparseVector& sv) const
	{
		check_compatibility(sv);

		T res = defval;
		auto intersect = intersect_keys(sv);
		for (auto i : intersect)
			res += values.at(i) * sv.values.at(i);

		return res;
	}

    SparseVector operator*(const T& c) const
	{
		SparseVector res = *this;
		for (auto& v : res.values)
			v.second *= c;

		res.clear_def_values();

		return res;
	}

    SparseVector operator/(const T& c) const
	{
		SparseVector res = *this;
		for (auto& v : res.values)
			v.second /= c;

		res.clear_def_values();

		return res;
	}

	// move operators
	SparseVector operator+(SparseVector&& sv) const
	{
		check_compatibility(sv);

		for (auto& v : values)
			sv[v.first] += v.second;

		sv.clear_def_values();
		
		return sv;
	}

    SparseVector operator-(SparseVector&& sv) const
	{
		check_compatibility(sv);

		auto un = union_keys(sv);
		for (auto& u : un)
			sv[u] = (*this)[u] - sv[u];

		sv.clear_def_values();
		
		return sv;
	}

    T operator*(SparseVector&& sv) const
	{
		check_compatibility(sv);

		T res = defval;
		auto intersect = intersect_keys(sv);
		for (auto i : intersect)
			res += values.at(i) * sv.values.at(i);

		return res;
	}

    SparseVector operator*(T&& c) const
	{
		SparseVector res = *this;
		for (auto& v : res.values)
			v.second *= c;

		res.clear_def_values();

		return res;
	}

    SparseVector operator/(T&& c) const
	{
		SparseVector res = *this;
		for (auto& v : res.values)
			v.second /= c;

		res.clear_def_values();

		return res;
	}

	T abs() const
	{
		T sum = defval;
		for (auto& v : values)
			sum += v.second * v.second;

		return std::sqrt(sum);
	}

	size_t get_len() const
	{
		return len;
	}

	void set_len(size_t len_)
	{
		len = len_;
		for (auto it = values.begin(); it != values.end(); )
		{
			if (it->first >= len)
			{
				values.erase(it, values.end());
				return;
			}
			else
				++it;
		}
	}

	T& operator[](size_t index)
	{
		if (len <= index)
			throw std::exception("Index out of bounds!");
		
		if (values.find(index) == values.end())
			values[index] = defval;

		return values[index];
	}

	const T& operator[](size_t index) const // does not increase container size
	{
		if (len <= index)
			throw std::exception("Index out of bounds!");
		
		if (values.find(index) == values.end())
			return defval;

		return values.at(index);
	}

	void clear()
	{
		values.clear();
	}

	friend std::ostream& operator<<(std::ostream& out, const SparseVector& sv)
	{
		if (sv.values.empty())
			out << "[ ]";
		else
		{
			out << "[ ..." << std::endl;
			for (auto& v : sv.values)
				out << "  " << v.first << ": " << v.second << std::endl;
			out << "  ... ]";
		}
		return out;
	}

	static bool keys_less_than(const pair<size_t, T>& v1, const pair<size_t, T>& v2)
	{
		return v1.first < v2.first;
	}

private:
	static T defval; // always additive identity ~ like 0
	size_t len;
    map<size_t, T> values;

	void check_compatibility(const SparseVector& sv) const
	{
		if (len != sv.len)
			throw std::exception("SparseVector dimension mismatch!");
	}

	void clear_def_values()
	{
		for (auto it = values.begin(); it != values.end(); )
		{
			if (it->second == std::numeric_limits<T>::epsilon()
				|| std::abs(it->second) < std::numeric_limits<T>::epsilon())
				it = values.erase(it);
			else
				++it;
		}
	}

	set<size_t> intersect_keys(const SparseVector& vs) const
	{
		set<size_t> intersect;
		auto first1 = values.cbegin();
		auto last1  = values.cend();
		auto first2 = vs.values.cbegin();
		auto last2  = vs.values.cend();
		auto d_first = std::inserter(intersect, intersect.begin());
		while (first1 != last1 && first2 != last2)
		{
			if (first1->first < first2->first)
				++first1;
			else
			{
				if (!(first2->first < first1->first))
					*d_first++ = (*first1++).first;
				++first2;
			}
		}
		return intersect;
	}

	set<size_t> difference_keys(const SparseVector& vs) const
	{
		set<size_t> diff;
		auto first1 = values.cbegin();
		auto last1  = values.cend();
		auto first2 = vs.values.cbegin();
		auto last2  = vs.values.cend();
		auto d_first = std::inserter(diff, diff.begin());
		while (first1 != last1)
		{
			if (first2 == last2)
			{
				auto keys_left = get_map_keys<size_t, T>(first1, last1);
				std::copy(keys_left.begin(), keys_left.end(), d_first);
				return diff;
			}
			if (first1->first < first2->first)
				*d_first++ = (*first1++).first;
			else
			{
				if (!(first2->first < first1->first))
					++first1;
				++first2;
			}
		}

		return diff;
	}

	set<size_t> union_keys(const SparseVector& vs) const
	{
		set<size_t> un;
		auto first1 = values.cbegin();
		auto last1  = values.cend();
		auto first2 = vs.values.cbegin();
		auto last2  = vs.values.cend();
		auto d_first = std::inserter(un, un.begin());
		for (; first1 != last1; ++d_first) {
			if (first2 == last2)
			{
				auto keys_left = get_map_keys<size_t, T>(first1, last1);
				std::copy(keys_left.begin(), keys_left.end(), d_first);
				return un;
			}
			if (first2->first < first1->first)
				*d_first = (*first2++).first;
			else
			{
				*d_first = first1->first;
				if (!(first1->first < first2->first))
					++first2;
				++first1;
			}
		}
		auto keys_left = get_map_keys<size_t, T>(first2, last2);
		std::copy(keys_left.begin(), keys_left.end(), d_first);

		return un;
	}
};

#endif //_SPARSEVECTOR_H