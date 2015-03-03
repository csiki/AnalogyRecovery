#ifndef _SPARSEVECTOR_H
#define _SPARSEVECTOR_H

#include <map>
#include <algorithm>

using std::map;

template <typename T>
class SparseVector
{
public:
	// ordinary operators TODO
    SparseVector operator+(const SparseVector& sv) const
	{
		return SparseVector();
	}

    SparseVector operator-(const SparseVector& sv) const
	{
		return SparseVector();
	}

    T operator*(const SparseVector& sv) const
	{
		return T();
	}

    SparseVector operator*(const T& c) const
	{
		return SparseVector();
	}

    SparseVector operator/(const T& c) const
	{
		return SparseVector();
	}

	// move operators TODO
	SparseVector operator+(SparseVector&& sv) const
	{
		return SparseVector();
	}

    SparseVector operator-(SparseVector&& sv) const
	{
		return SparseVector();
	}

    T operator*(SparseVector&& sv) const
	{
		return SparseVector();
	}

    SparseVector operator*(T&& c) const
	{
		return SparseVector();
	}

    SparseVector operator/(T&& c) const
	{
		return SparseVector();
	}

	T abs() const
	{
		return std::sqrt(
			std::for_each(values.begin(), values.end(),
				[] (const T& val) { square_sum += val * val; })
		);
	}

	size_t get_len() const
	{
		return len;
	}

	void set_val(T val, size_t index)
	{
		values[index] = val;
	}

private:
	T defval;
	size_t len;
    map<size_t, T> values;
};

#endif //_SPARSEVECTOR_H