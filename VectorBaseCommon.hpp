#ifndef VECTORBASECOMMON_HPP
#define VECTORBASECOMMON_HPP
#include <stdexcept>

namespace ft
{
class VectorBaseCommon
{
protected:
	VectorBaseCommon(){}
	void	throwLengthError() const
	{
		std::__throw_length_error("vector");
	}
	void	throwOutOfRange() const
	{
		std::__throw_out_of_range("vector");
	}
};
}
#endif