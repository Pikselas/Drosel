#pragma once
#include<iterator>
#include<iostream>
namespace ksTools
{
	inline auto seek_itr_forward(auto beg, auto end, size_t seek_by)
	{
		if ( static_cast<size_t>(std::distance(beg, end)) >= seek_by)
		{
			return beg + seek_by;
		}
		else
		{
			return end;
		}
		return end;
	}
	inline auto seek_itr_backward(auto beg, auto end, size_t seek_by)
	{
		if (static_cast<size_t>(std::distance(beg, end)) >= seek_by)
		{
			return end - seek_by;
		}
		else
		{
			return beg;
		}
		return beg;
	}
}