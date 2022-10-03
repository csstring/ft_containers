#include "VectorBaseCommon.hpp"

void	ft::VectorBaseCommon::throwLengthError() const
{
	std::__throw_length_error("vector");
}
void	ft::VectorBaseCommon::throwOutOfRange() const
{
	std::__throw_out_of_range("vector");
}