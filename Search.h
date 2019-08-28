#pragma once
#include "framework.h"

/* 
	Performs at binary search on a sorted list to find out how many entries in
	the list are smaller than value. 
*/
inline size_t FindRank(const std::vector<size_t>& numbers, size_t value)
{
	size_t leftOffset = 0;
	size_t rightOffset = numbers.size();
	size_t searchIndex = 0;

	while (leftOffset < rightOffset)
	{
		searchIndex = (leftOffset + rightOffset) / 2;
		if (numbers.at(searchIndex) < value)
			leftOffset = searchIndex + 1;
		else
			rightOffset = searchIndex;
	}

	return leftOffset;
}