#ifndef PAIR_HPP
#define PAIR_HPP
#include <vector>
namespace ft{

template <class T1, class T2> struct pair
{
	T1 first;
	T2 second;

	pair(){}
	pair(T1 const & t1, T2 const& t2) : first(t1), second(t2){}
	template <class U1,class U2>
	pair(const pair<U1, U2>& copy) : first(copy.first), second(copy.second){}
	pair& operator=(pair const& ref)
	{
		this->first = ref.first;
		this->second = ref.second;
		return (*this);
	}
};

template <class U1, class U2 >
bool operator==(const pair<U1,U2>& lhs, const pair<U1,U2>& rhs )
{ return (lhs.first == rhs.first && lhs.second == rhs.second); }

template <class T1,class T2>
pair<T1,T2> make_pair (T1 x, T2 y)
{
	return ( pair<T1,T2>(x,y) );
}
}
#endif