#pragma once
#ifndef Algorithm_h__
#define Algorithm_h__

namespace Algorithm
{
	template<typename T>
	void swap(T& val1, T& val2) {
		T tmp = val2;
		val2 = val1;
		val1 = tmp;
	}

	template<typename Iter_t, typename Cmpfunc_t>
	void quickSort(Iter_t start, Iter_t end, Cmpfunc_t&& cmpfunc) {
		if (end == start || end == ++start) {
			return;
		}
		--start;
		Iter_t i = start, j = end;
		--j;
		Iter_t pivot = start;
		do {
			while (i != j && !cmpfunc(*pivot, *i)) {
				++i;
			}
			while (i != j && !cmpfunc(*j, *pivot)) {
				--j;
			}
			swap(*i, *j);
		} while (i != j);
		if (!cmpfunc(*i, *pivot)) {
			--i;
		}
		swap(*i, *pivot);
		pivot = i;
		quickSort(start, pivot, cmpfunc);
		quickSort(++pivot, end, cmpfunc);
	}

}

#endif // Algorithm_h__