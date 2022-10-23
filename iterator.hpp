#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "utility.hpp"
namespace ft
{
template <class Iter, bool>
struct iterator_traits_form {};

template <class Iter>
struct iterator_traits_form<Iter, true>
{
    typedef typename Iter::difference_type   difference_type;
    typedef typename Iter::value_type        value_type;
    typedef typename Iter::pointer           pointer;
    typedef typename Iter::reference         reference;
    typedef typename Iter::iterator_category iterator_category;
};

template <typename _Iter, bool>
struct _iterator_traits {};

template <class Iter>
struct _iterator_traits<Iter, true> : iterator_traits_form<Iter, 
is_same<typename Iter::iterator_category, std::input_iterator_tag>::value ||
is_same<typename Iter::iterator_category, std::output_iterator_tag>::value ||
is_same<typename Iter::iterator_category, std::forward_iterator_tag>::value ||
is_same<typename Iter::iterator_category, std::bidirectional_iterator_tag>::value ||
is_same<typename Iter::iterator_category, std::random_access_iterator_tag>::value>
{};

template <class>
struct __void_t { typedef void type; };
template <class _Tp>
struct __has_iterator_typedefs
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename __void_t<typename _Up::iterator_category>::type* = 0,
                                            typename __void_t<typename _Up::difference_type>::type* = 0,
                                            typename __void_t<typename _Up::value_type>::type* = 0,
                                            typename __void_t<typename _Up::reference>::type* = 0,
                                            typename __void_t<typename _Up::pointer>::type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0,0,0,0,0)) == 1;
};
template <class _Iter>
struct iterator_traits
    : _iterator_traits<_Iter, __has_iterator_typedefs<_Iter>::value> {
};
template <class T>
struct iterator_traits<T*>
{
    typedef ptrdiff_t difference_type;
    typedef typename remove_cv<T>::type value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;
};

template <class Iter>
class vector_iterator
    : public std::iterator<typename iterator_traits<Iter>::iterator_category,
                      typename iterator_traits<Iter>::value_type,
                      typename iterator_traits<Iter>::difference_type,
                      typename iterator_traits<Iter>::pointer,
                      typename iterator_traits<Iter>::reference>
                      {};

template <class Iter>
class _reverse_iterator
    : public std::iterator<typename iterator_traits<Iter>::iterator_category,
                      typename iterator_traits<Iter>::value_type,
                      typename iterator_traits<Iter>::difference_type,
                      typename iterator_traits<Iter>::pointer,
                      typename iterator_traits<Iter>::reference>
{
private:
    Iter t;
protected:
    Iter current;
public:
    typedef Iter                                            iterator_type;
    typedef typename iterator_traits<Iter>::difference_type difference_type;
    typedef typename iterator_traits<Iter>::reference       reference;
    typedef typename iterator_traits<Iter>::pointer         pointer;

    _reverse_iterator() : t(), current() {}
    explicit _reverse_iterator(Iter x) : t(x), current(x) {}
    template <class U>
    _reverse_iterator(const _reverse_iterator<U>& u) : t(u.base()), current(u.base()) {}
    template <class U>
    _reverse_iterator& operator=(const _reverse_iterator<U>& u)
            { t = current = u.base(); return *this; }
    Iter base() const {return current;}
    reference operator*() const {Iter tmp = current; return *--tmp;}
    pointer  operator->() const {return &(operator*());}
    _reverse_iterator& operator++() {--current; return *this;}
    _reverse_iterator  operator++(int) {_reverse_iterator tmp(*this); --current; return tmp;}
    _reverse_iterator& operator--() {++current; return *this;}
    _reverse_iterator  operator--(int) {_reverse_iterator tmp(*this); ++current; return tmp;}
    _reverse_iterator  operator+ (difference_type n) const {return _reverse_iterator(current - n);}
    _reverse_iterator& operator+=(difference_type n) {current -= n; return *this;}
    _reverse_iterator  operator- (difference_type n) const {return _reverse_iterator(current + n);}
    _reverse_iterator& operator-=(difference_type n) {current += n; return *this;}
    reference         operator[](difference_type n) const {return *(*this + n);}
};

template <class Iter1, class Iter2>
bool operator==(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() == y.base();
}

template <class Iter1, class Iter2>
bool operator<(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() > y.base();
}

template <class Iter1, class Iter2>
bool operator!=(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() != y.base();
}

template <class Iter1, class Iter2>
bool operator>(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() < y.base();
}

template <class Iter1, class Iter2>
bool operator>=(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() <= y.base();
}

template <class Iter1, class Iter2>
bool operator<=(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return x.base() >= y.base();
}

template <class Iter1, class Iter2>
typename _reverse_iterator<Iter1>::difference_type
operator-(const _reverse_iterator<Iter1>& x, const _reverse_iterator<Iter2>& y)
{
    return y.base() - x.base();
}

template <class Iter>
_reverse_iterator<Iter> operator+(typename _reverse_iterator<Iter>::difference_type n, const _reverse_iterator<Iter>& x)
{
    return _reverse_iterator<Iter>(x.base() - n);
}

template <typename _Iter>
class __vector_iterator {
public:
  typedef _Iter iterator_type;
  typedef typename ft::iterator_traits<iterator_type>::iterator_category iterator_category;
  typedef typename ft::iterator_traits<iterator_type>::value_type value_type;
  typedef typename ft::iterator_traits<iterator_type>::difference_type difference_type;
  typedef typename ft::iterator_traits<iterator_type>::pointer pointer;
  typedef typename ft::iterator_traits<iterator_type>::reference reference;

private:
  iterator_type __it;

public:
  __vector_iterator() {}
  template <typename _U>
  __vector_iterator( const __vector_iterator<_U>& __u) : __it(__u.base()) {}
  __vector_iterator(iterator_type __x) : __it(__x) {}
  const iterator_type& base() const { return __it; }
  // operator
  reference operator*() const { return *__it; }
  pointer operator->() const { return __it; }
  __vector_iterator& operator++() 
  {
    ++__it;
    return *this;
  }
  __vector_iterator operator++(int) 
  {
    __vector_iterator __tmp(*this);
    ++(*this);
    return __tmp;
  }
  __vector_iterator& operator--() 
  {
    --__it;
    return *this;
  }
  __vector_iterator operator--(int) 
  {
    __vector_iterator __tmp(*this);
    --(*this);
    return __tmp;
  }
  __vector_iterator& operator+=(difference_type __n) 
  {
    __it += __n;
    return *this;
  }
  __vector_iterator operator+(difference_type __n) const 
  {
    __vector_iterator __w(*this);
    __w += __n;
    return __w;
  }
  __vector_iterator& operator-=(difference_type __n) 
  {
    __it -= __n;
    return *this;
  }
  __vector_iterator operator-(difference_type __n) const 
  {
    __vector_iterator __w(*this);
    __w -= __n;
    return __w;
  }
  reference operator[](difference_type __n) { return __it[__n]; }
};

template <typename _Iter1, typename _Iter2>
bool operator==(const __vector_iterator<_Iter1>& __lhs,
                const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() == __rhs.base();
}

template <typename _Iter1, typename _Iter2>
bool operator!=(const __vector_iterator<_Iter1>& __lhs,
                const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() != __rhs.base();
}

template <typename _Iter1, typename _Iter2>
bool operator<(const __vector_iterator<_Iter1>& __lhs,
               const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() < __rhs.base();
}

template <typename _Iter1, typename _Iter2>
bool operator>(const __vector_iterator<_Iter1>& __lhs,
               const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() > __rhs.base();
}

template <typename _Iter1, typename _Iter2>
bool operator>=(const __vector_iterator<_Iter1>& __lhs,
                const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() >= __rhs.base();
}

template <typename _Iter1, typename _Iter2>
bool operator<=(const __vector_iterator<_Iter1>& __lhs,
                const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() <= __rhs.base();
}

template <typename _Iter1, typename _Iter2>
typename __vector_iterator<_Iter1>::difference_type operator-(
    const __vector_iterator<_Iter1>& __lhs,
    const __vector_iterator<_Iter2>& __rhs) {
  return __lhs.base() - __rhs.base();
}

template <typename _Iter>
__vector_iterator<_Iter> operator+(
    typename __vector_iterator<_Iter>::difference_type __n,
    const __vector_iterator<_Iter>& __it) {
  return __vector_iterator<_Iter>(__it.base() + __n);
}

};
#endif