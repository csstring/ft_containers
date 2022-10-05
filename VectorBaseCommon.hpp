#ifndef VECTORBASECOMMON_HPP
#define VECTORBASECOMMON_HPP
#include <stdexcept>

namespace ft
{
class VectorBaseCommon
{
protected:
	VectorBaseCommon(){}
	void	throwLengthError() const;
	void	throwOutOfRange() const;
};
}
#endif