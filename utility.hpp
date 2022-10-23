#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace ft{

template <class T1, class T2> struct pair
{
	typedef T1 first_type;
	typedef T2 second_type;
	
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

template <class T1,class T2>
pair<T1,T2> make_pair (T1 x, T2 y)
{
	return ( pair<T1,T2>(x,y) );
}

template <class T1, class T2>
  bool operator== (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return lhs.first==rhs.first && lhs.second==rhs.second; }

template <class T1, class T2>
  bool operator!= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(lhs==rhs); }

template <class T1, class T2>
  bool operator<  (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return lhs.first<rhs.first || (!(rhs.first<lhs.first) && lhs.second<rhs.second); }

template <class T1, class T2>
  bool operator<= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(rhs<lhs); }

template <class T1, class T2>
  bool operator>  (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return rhs<lhs; }

template <class T1, class T2>
  bool operator>= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(lhs<rhs); }
template <bool Cond, class T = void> struct enable_if
{};

template <class T> struct enable_if<true, T>
{
	typedef T type;
};

template <class Cond> struct false_type
{
	static const Cond value = false;
};
template <class Cond> struct true_type
{
	static const Cond value = true;
};

template <class _Tp> struct is_integral                     : public false_type<bool> {};
template <>          struct is_integral<bool>               : public true_type<bool> {};
template <>          struct is_integral<char>               : public true_type<bool> {};
template <>          struct is_integral<signed char>        : public true_type<bool> {};
template <>          struct is_integral<unsigned char>      : public true_type<bool> {};
template <>          struct is_integral<wchar_t>            : public true_type<bool> {};
template <>          struct is_integral<short>              : public true_type<bool> {};
template <>          struct is_integral<unsigned short>     : public true_type<bool> {};
template <>          struct is_integral<int>                : public true_type<bool> {};
template <>          struct is_integral<unsigned int>       : public true_type<bool> {};
template <>          struct is_integral<long>               : public true_type<bool> {};
template <>          struct is_integral<unsigned long>      : public true_type<bool> {};
template <>          struct is_integral<long long>          : public true_type<bool> {};
template <>          struct is_integral<unsigned long long> : public true_type<bool> {};

template <class T1, class T2>
struct is_same : public false_type<bool> {};
template <class T1>
struct is_same<T1, T1> : public true_type<bool> {};

template <class T>
struct remove_const {typedef T type;};
template <class T>
struct remove_const<const T> {typedef T type;};
template <class T>
struct remove_volatile {typedef T type;};
template <class T>
struct remove_volatile<volatile T> {typedef T type;};
template <class _Tp> 
struct remove_reference        {typedef _Tp type;};
template <class _Tp> 
struct remove_reference<_Tp&>  {typedef _Tp type;};
template <class T> 
struct remove_cv {
	typedef typename remove_volatile<typename remove_const<T>::type>::type type ;
};

template <class InputIterator1, class InputIterator2>
bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
		if (!(*first1 == *first2))
			return (false);
	return (true);
}

template <class InputIterator1, class InputIterator2>
bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
								InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1)
	{
		if (first2 == last2 || *first2 < *first1)
			return (false);
		else if (*first1 < *first2)
			return (true);
		++first1;
		++first2;
	}
	return (first2 != last2);
}
template<typename _Tp>
struct _Identity
{
	_Tp& operator()(_Tp& __x) const { return __x; }
	const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class pair>
struct _Select1st
{
	typename pair::first_type &operator()(pair &__x) const { return __x.first; }
	const typename pair::first_type &operator()(const pair &__x) const { return __x.first;
}

};


}
#endif