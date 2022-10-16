#ifndef MAP_HPP
#define MAP_HPP
#include "tree.hpp"

namespace ft{
	
template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>, 
			typename _Alloc = std::allocator<ft::pair<const _Key, _Tp> > >
class map
{
public:
    typedef _Key						key_type;
    typedef _Tp							mapped_type;
    typedef ft::pair<const _Key, _Tp>	value_type;
    typedef _Compare					key_compare;
	typedef _Alloc						allocator_type;
private:
    typedef typename _Alloc::value_type		_Alloc_value_type;

public:

	class value_compare : public std::binary_function<value_type, value_type, bool>
	{
		friend class map<_Key, _Tp, key_compare, _Alloc>;
	protected:
		key_compare comp;
		value_compare(key_compare __c) : comp(__c) {}
	public:
		bool operator()(const value_type& __x, const value_type& __y) const
			{ return comp(__x.first, __y.first); }
	};

private:
	typedef typename _Alloc::template rebind<value_type>::other _Pair_alloc_type;
   	typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
	       key_compare, _Pair_alloc_type> _Rep_type;
	_Rep_type _M_t;
//	typedef __gnu_cxx::__alloc_traits<_Pair_alloc_type> _Alloc_traits;
public:
	typedef typename allocator_type::pointer		 pointer;
	typedef typename allocator_type::const_pointer	 const_pointer;
	typedef typename allocator_type::reference		 reference;
	typedef typename allocator_type::const_reference	 const_reference;
	typedef typename _Rep_type::iterator		 iterator;
	typedef typename _Rep_type::const_iterator	 const_iterator;
	typedef typename _Rep_type::size_type		 size_type;
	typedef typename _Rep_type::difference_type	 difference_type;
	typedef typename _Rep_type::reverse_iterator	 reverse_iterator;
	typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
	
// canonical form	
	map(const key_compare& __comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _M_t(__comp, alloc) {}
	map(const map& __x) : _M_t(__x._M_t) {}
	template<typename _InputIterator>
	map(_InputIterator __first, _InputIterator __last, 
	const key_compare& __comp = key_compare(), const allocator_type& alloc = allocator_type())
	: _M_t(__comp, _Pair_alloc_type(alloc))
	{ _M_t._M_insert_range_unique(__first, __last); }
	map& operator= (const map& x) {_M_t = x._M_t; return *this;}
// iterators
	allocator_type get_allocator() const  { return allocator_type(_M_t.get_allocator()); }
	iterator begin() { return _M_t.begin(); }
	const_iterator begin() const { return _M_t.begin(); }
	iterator end() { return _M_t.end(); }
	const_iterator end() const { return _M_t.end(); }
	reverse_iterator rbegin() { return _M_t.rbegin(); }
	const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
	reverse_iterator rend() { return _M_t.rend(); }
	const_reverse_iterator rend() const { return _M_t.rend(); }
//capacity
	bool empty() const { return _M_t.empty(); }
	size_type size() const { return _M_t.size(); }
	size_type max_size() const { return _M_t.max_size(); }
//Element access:
	mapped_type& operator[](const key_type& k)
	{
		iterator __i = lower_bound(__k);
		if (__i == end() || key_comp()(__k, (*__i).first))
	  		__i = insert(__i, value_type(__k, mapped_type()));
		return (*__i).second;
	}
//Modifiers:
	iterator insert(iterator __position, const value_type& __x)
	{ return _M_t._M_insert_unique_(__position, __x); }
	std::pair<iterator, bool> insert(const value_type& __x)
	{ return _M_t._M_insert_unique(__x); }
	template<typename _InputIterator>
	void insert(_InputIterator __first, _InputIterator __last)
	{ _M_t._M_insert_range_unique(__first, __last); }

	void erase(iterator __position)
	{ _M_t.erase(__position); }
	size_type erase(const key_type& __x)
	{ return _M_t.erase(__x); }
	void erase(iterator __first, iterator __last)
	{ _M_t.erase(__first, __last); }

	void swap(map& __x) { _M_t.swap(__x._M_t); }
	void clear() { _M_t.clear(); }
}

}
#endif