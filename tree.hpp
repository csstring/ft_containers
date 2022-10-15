#ifndef TREE_HPP
#define TREE_HPP
#include "iterator.hpp"

namespace ft{
enum _Rb_tree_color { _S_red = false, _S_black = true };

struct _Rb_tree_node_base
{
	typedef _Rb_tree_node_base* _Base_ptr;
	typedef const _Rb_tree_node_base* _Const_Base_ptr;

	_Rb_tree_color	_M_color;
	_Base_ptr		_M_parent;
	_Base_ptr		_M_left;
	_Base_ptr		_M_right;

	static _Base_ptr _S_minimum(_Base_ptr x)
	{
		while (x->_M_left != 0) 
			x = x->_M_left;
		return x;
	}

	static _Const_Base_ptr _S_minimum(_Const_Base_ptr x)
	{
		while (x->_M_left != 0)
			x = x->_M_left;
		return x;
	}

	static _Base_ptr _S_maximum(_Base_ptr x)
	{
		while (x->_M_right != 0)
			x = x->_M_right;
		return x;
	}

	static _Const_Base_ptr _S_maximum(_Const_Base_ptr x)
	{
		while (x->_M_right != 0)
			x = x->_M_right;
		return x;
	}
};

template<typename _Key_compare>
struct _Rb_tree_key_compare
{
	_Key_compare		_M_key_compare;

	_Rb_tree_key_compare() : _M_key_compare() {}
	_Rb_tree_key_compare(const _Key_compare& comp) 
	: _M_key_compare(comp){}
};

struct _Rb_tree_header
{
	_Rb_tree_node_base	_M_header;
	size_t		_M_node_count; // Keeps track of size of tree.

	_Rb_tree_header()
	{
		_M_header._M_color = _S_red;
		_M_reset();
	}
	void _M_move_data(_Rb_tree_header& from)
	{
		_M_header._M_color = from._M_header._M_color;
		_M_header._M_parent = from._M_header._M_parent;
		_M_header._M_left = from._M_header._M_left;
		_M_header._M_right = from._M_header._M_right;
		_M_header._M_parent->_M_parent = &_M_header;
		_M_node_count = from._M_node_count;
		from._M_reset();
	}

	void _M_reset()
	{
		_M_header._M_parent = 0;
		_M_header._M_left = &_M_header;
		_M_header._M_right = &_M_header;
		_M_node_count = 0;
	}
};

template<typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base
{
	typedef _Rb_tree_node<_Val>* _Link_type;

	_Val _M_value_field;

	_Val* _M_valptr() { return &_M_value_field; }
	const _Val* _M_valptr() const { return &_M_value_field; }
};

_Rb_tree_node_base* _Rb_tree_increment(_Rb_tree_node_base* __x) throw ();
const _Rb_tree_node_base* _Rb_tree_increment(const _Rb_tree_node_base* __x) throw ();
_Rb_tree_node_base* _Rb_tree_decrement(_Rb_tree_node_base* __x) throw ();
const _Rb_tree_node_base* _Rb_tree_decrement(const _Rb_tree_node_base* __x) throw ();

template<typename _Tp>
struct _Rb_tree_iterator
{
	typedef _Tp  value_type;
	typedef _Tp& reference;
	typedef _Tp* pointer;

	typedef std::bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t			 difference_type;

	typedef _Rb_tree_iterator<_Tp>		_Self;
	typedef _Rb_tree_node_base::_Base_ptr	_Base_ptr;
	typedef _Rb_tree_node<_Tp>*		_Link_type;

	_Base_ptr _M_node;
	
	_Rb_tree_iterator() : _M_node() { }
	
	explicit _Rb_tree_iterator(_Base_ptr __x) : _M_node(__x) {}
	
	reference operator*() const
	{ return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

	pointer operator->() const
	{ return static_cast<_Link_type> (_M_node)->_M_valptr(); }

	_Self& operator++()
	{
		_M_node = _Rb_tree_increment(_M_node);
		return *this;
	}

	_Self operator++(int)
	{	
		_Self __tmp = *this;
		_M_node = _Rb_tree_increment(_M_node);
		return __tmp;
	}

	_Self& operator--()
	{
		_M_node = _Rb_tree_decrement(_M_node);
		return *this;
	}

	_Self operator--(int)
	{
		_Self __tmp = *this;
		_M_node = _Rb_tree_decrement(_M_node);
		return __tmp;
	}

	friend bool operator==(const _Self& __x, const _Self& __y) 
	{ return __x._M_node == __y._M_node; }
	friend bool operator!=(const _Self& __x, const _Self& __y)
	{ return !(__x._M_node == __y._M_node); }

};

template<typename _Tp>
struct _Rb_tree_const_iterator
{
	typedef _Tp	 value_type;
	typedef const _Tp& reference;
	typedef const _Tp* pointer;
	typedef _Rb_tree_iterator<_Tp> iterator;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t			 difference_type;
	
	typedef _Rb_tree_const_iterator<_Tp>		_Self;
	typedef _Rb_tree_node_base::_Const_Base_ptr	_Base_ptr;
	typedef const _Rb_tree_node<_Tp>*			_Link_type;
	_Base_ptr _M_node;

	_Rb_tree_const_iterator() : _M_node() { }

	explicit _Rb_tree_const_iterator(_Base_ptr __x) : _M_node(__x) {}

	_Rb_tree_const_iterator(const iterator& __it) : _M_node(__it._M_node) { }

	iterator _M_const_cast() const
	{ return iterator(const_cast<typename iterator::_Base_ptr>(_M_node)); }

    reference operator*() const
	{ return *static_cast<_Link_type>(_M_node)->_M_valptr(); }

    pointer operator->() const
    { return static_cast<_Link_type>(_M_node)->_M_valptr(); }

	_Self& operator++()
	{
		_M_node = _Rb_tree_increment(_M_node);
		return *this;
	}

	_Self operator++(int)
	{
		_Self __tmp = *this;
		_M_node = _Rb_tree_increment(_M_node);
		return __tmp;
	}

	_Self& operator--()
	{
		_M_node = _Rb_tree_decrement(_M_node);
		return *this;
	}

	_Self operator--(int)
	{
		_Self __tmp = *this;
		_M_node = _Rb_tree_decrement(_M_node);
		return __tmp;
	}
	friend bool operator==(const _Self& __x, const _Self& __y) 
	{ return __x._M_node == __y._M_node; }
	friend bool operator!=(const _Self& __x, const _Self& __y)
	{ return !(__x._M_node == __y._M_node); }
};

void _Rb_tree_insert_and_rebalance(const bool __insert_left,
				_Rb_tree_node_base* __x,
				_Rb_tree_node_base* __p,
				_Rb_tree_node_base& __header) throw ();

_Rb_tree_node_base* _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z, 
						_Rb_tree_node_base& __header) throw ();

template<typename _Key, typename _Val, typename _KeyOfValue, 
			typename _Compare, typename _Alloc = std::allocator<_Val> >
class _Rb_tree
{
	typedef typename _Alloc::template rebind<_Rb_tree_node<_Val>::other _Node_allocator;
//    typedef __gnu_cxx::__alloc_traits<_Node_allocator> allocator_type;

protected:
    typedef _Rb_tree_node_base* 		_Base_ptr;
    typedef const _Rb_tree_node_base* 	_Const_Base_ptr;
    typedef _Rb_tree_node<_Val>* 		_Link_type;
    typedef const _Rb_tree_node<_Val>*	_Const_Link_type;

private:
public:
    typedef _Key 				key_type;
	typedef _Val 				value_type;
	typedef value_type* 			pointer;
    typedef const value_type* 		const_pointer;
    typedef value_type& 			reference;
    typedef const value_type& 		const_reference;
    typedef size_t 				size_type;
    typedef ptrdiff_t 			difference_type;
    typedef _Alloc 				allocator_type;

	_Node_allocator& _M_get_Node_allocator() { return this->_M_impl; }
	const _Node_allocator& _M_get_Node_allocator() const { return this->_M_impl; }
	allocator_type get_allocator() const { return allocator_type(_M_get_Node_allocator()); }
protected:
	_Link_type _M_get_node() { return allocator_type::allocate(1); }
	void _M_put_node(_Link_type __p) { allocator_type::deallocate(__p, 1); }
	void _M_construct_node(_Link_type __node, const value_type& __x)
	{
		try
		{ 
			get_allocator().construct(__node->_M_valptr(), __x); 
		}
		catch(...)
		{
			_M_put_node(__node);
			throw;
		}
	}
	_Link_type _M_create_node(const value_type& __x)
	{
		_Link_type __tmp = _M_get_node();
		_M_construct_node(__tmp, __x);
		return __tmp;
	}

	void _M_destroy_node(_Link_type __p) { get_allocator().destroy(__p->_M_valptr()); }
	void _M_drop_node(_Link_type __p) 
	{
		_M_destroy_node(__p);
		_M_put_node(__p);
	}
	template<bool _MoveValue>
	_Link_type _M_clone_node(_Link_type __x)
	{
		_Link_type __tmp = this->_M_create_node(*__x->_M_valptr())
		__tmp->_M_color = __x->_M_color;
		__tmp->_M_left = 0;
		__tmp->_M_right = 0;
		return __tmp;
	}

	template<typename _Key_compare>
	struct _Rb_tree_impl: public _Node_allocator
						, public _Rb_tree_key_compare<_Key_compare>
						, public _Rb_tree_header
	{
		typedef _Rb_tree_key_compare<_Key_compare> _Base_key_compare;

		_Rb_tree_impl() : _Node_allocator() {}
		_Rb_tree_impl(const _Rb_tree_impl& __x) 
		: _Node_allocator(__x) , _Base_key_compare(__x._M_key_compare) , _Rb_tree_header() {}
		_Rb_tree_impl(const _Key_compare& __comp, const _Node_allocator& __a)
		: _Node_allocator(__a), _Base_key_compare(__comp) {}
	};
	_Rb_tree_impl<_Compare> _M_impl;
	_Base_ptr& _M_root() { return this->_M_impl._M_header._M_parent; }
	_Const_Base_ptr _M_root() const { return this->_M_impl._M_header._M_parent; }
	_Base_ptr& _M_leftmost() { return this->_M_impl._M_header._M_left; }
	_Const_Base_ptr _M_leftmost() const { return this->_M_impl._M_header._M_left; }
	_Base_ptr& _M_rightmost() { return this->_M_impl._M_header._M_right; }
	_Const_Base_ptr _M_rightmost() const { return this->_M_impl._M_header._M_right; }
	_Link_type _M_mbegin() const { return static_cast<_Link_type>(this->_M_impl._M_header._M_parent); }
	_Link_type _M_begin() { return _M_mbegin(); }
	_Const_Link_type _M_begin() const 
	{ return static_cast<_Const_Link_type>(this->_M_impl._M_header._M_parent); }
	_Base_ptr _M_end() { return &this->_M_impl._M_header; }
	_Const_Base_ptr _M_end() const { return &this->_M_impl._M_header; }
	
	static const _Key&_S_key(_Const_Link_type __x)
	{ return _KeyOfValue()(*__x->_M_valptr()); }

	static _Link_type _S_left(_Base_ptr __x) 
	{ return static_cast<_Link_type>(__x->_M_left); }

	static _Const_Link_type _S_left(_Const_Base_ptr __x) 
	{ return static_cast<_Const_Link_type>(__x->_M_left); }

	static _Link_type _S_right(_Base_ptr __x) 
	{ return static_cast<_Link_type>(__x->_M_right); }

	static _Const_Link_type _S_right(_Const_Base_ptr __x) 
	{ return static_cast<_Const_Link_type>(__x->_M_right); }

	static const _Key& _S_key(_Const_Base_ptr __x)
	{ return _S_key(static_cast<_Const_Link_type>(__x)); }

	static _Base_ptr_S_minimum(_Base_ptr __x) 
	{ return _Rb_tree_node_base::_S_minimum(__x); }

	static _Const_Base_ptr _S_minimum(_Const_Base_ptr __x) 
	{ return _Rb_tree_node_base::_S_minimum(__x); }

	static _Base_ptr _S_maximum(_Base_ptr __x) 
	{ return _Rb_tree_node_base::_S_maximum(__x); }

	static _Const_Base_ptr _S_maximum(_Const_Base_ptr __x) 
	{ return _Rb_tree_node_base::_S_maximum(__x); }
public:
	typedef _Rb_tree_iterator<value_type>	 iterator;
	typedef _Rb_tree_const_iterator<value_type> const_iterator;
	typedef typename ft::_reverse_iterator<iterator> reverse_iterator;
	typedef typename ft::_reverse_iterator<const iterator> reverse_iterator;

	ft::pair<_Base_ptr, _Base_ptr> _M_get_insert_unique_pos(const key_type& __k);
    ft::pair<_Base_ptr, _Base_ptr> _M_get_insert_equal_pos(const key_type& __k);

	iterator _M_insert_(_Base_ptr __x, _Base_ptr __y, const value_type& __v);
    iterator _M_insert_lower(_Base_ptr __y, const value_type& __v);
	iterator _M_insert_equal_lower(const value_type& __x);

	enum { __as_lvalue, __as_rvalue };
	
	template<bool _MoveValues>
	_Link_type _M_copy(_Link_type, _Base_ptr);
	
	template<bool _MoveValues>
	_Link_type _M_copy(const _Rb_tree& __x)
	{
		_Link_type __root = _M_copy<_MoveValues>(__x._M_mbegin(), _M_end());
		_M_leftmost() = _S_minimum(__root);
		_M_rightmost() = _S_maximum(__root);
		_M_impl._M_node_count = __x._M_impl._M_node_count;
		return __root;
	}
	_Link_type _M_copy(const _Rb_tree& __x)
    {
		return _M_copy<__as_lvalue>(__x);
	}

	void _M_erase(_Link_type __x);

	iterator _M_lower_bound(_Link_type __x, _Base_ptr __y, const _Key& __k);
	const_iterator _M_lower_bound(_Const_Link_type __x, _Const_Base_ptr __y, const _Key& __k) const;
	iterator _M_upper_bound(_Link_type __x, _Base_ptr __y, const _Key& __k);
	const_iterator	_M_upper_bound(_Const_Link_type __x, _Const_Base_ptr __y, const _Key& __k) const;

public:
	_Rb_tree(){};
	_Rb_tree(const _Compare& __comp, const allocator_type& __a = allocator_type())
	: _M_impl(__comp, _Node_allocator(__a)) {}
	_Rb_tree(const _Rb_tree& __x) : _M_impl(__x._M_impl)
	{
		if (__x._M_root() != 0)
			_M_root() = _M_copy(__x);
	}
	~_Rb_tree() { _M_erase(_M_begin()); }
    _Rb_tree& operator=(const _Rb_tree& __x)
	{
		if (this != __x)
		{
			this->clear();
			_M_impl._M_key_compare = __x._M_impl._M_key_compare;
			if (__x.M_root != 0)
				_M_root() = _M_copy(__x);
		}
	}
	_Compare key_comp() const { return _M_impl._M_key_compare; }
	iterator begin() { return iterator(this->_M_impl._M_header._M_left); }
	const_iterator begin() const
	{ return const_iterator(this->_M_impl._M_header._M_left); }
	iterator end()
	{ return iterator(&this->_M_impl._M_header); }
	const_iterator end() const
	{ return const_iterator(&this->_M_impl._M_header); }
	reverse_iterator rbegin()
	{ return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const
	{ return const_reverse_iterator(end()); }
	reverse_iterator rend()
	{ return reverse_iterator(begin()); }
	const_reverse_iterator rend() const
	{ return const_reverse_iterator(begin()); }
	bool empty() const
	{ return _M_impl._M_node_count == 0; }
	size_type size() const
	{ return _M_impl._M_node_count; }
	size_type max_size() const
	{ return get_allocator().max_size();}
	void swap(_Rb_tree& t);
//insert/erase.
	pair<iterator, bool> _M_insert_unique(const value_type& __x);
	iterator _M_insert_equal(const value_type& __x);
	iterator _M_insert_unique_(const_iterator __pos, const value_type& __x);
	iterator _M_insert_equal_(const_iterator __pos, const value_type& __x);
	
	template<typename _InputIterator>
	void _M_insert_range_unique(_InputIterator __first, _InputIterator __last)
	{
		for (; __first != __last; ++__first)
			_M_insert_unique_(end(), *__first);
	}
	template<typename _InputIterator>
	void _M_insert_range_equal(_InputIterator __first, _InputIterator __last)
	{
		for (; __first != __last; ++__first)
			_M_insert_equal_(end(), *__first);
	}
private:
	void _M_erase_aux(const_iterator __position);
	void _M_erase_aux(const_iterator __first, const_iterator __last);
public:
	void erase(iterator __position) { _M_erase_aux(__position); }
	void erase(const_iterator __position) { _M_erase_aux(__position); }
	void erase(iterator __first, iterator __last) { _M_erase_aux(__first, __last); }
	void erase(const_iterator __first, const_iterator __last) { _M_erase_aux(__first, __last); }
	
	size_type erase(const key_type& __x);
	void clear() 
	{
		_M_erase(_M_begin());
		_M_impl._M_reset();
	}
// Set operations.
	iterator find(const key_type& __k);
	const_iterator find(const key_type& __k) const;

    size_type count(const key_type& __k) const;

	iterator lower_bound(const key_type& __k)
	{ return _M_lower_bound(_M_begin(), _M_end(), __k); }
	const_iteratorlower_bound(const key_type& __k) const
	{ return _M_lower_bound(_M_begin(), _M_end(), __k); }

	iterator upper_bound(const key_type& __k)
	{ return _M_upper_bound(_M_begin(), _M_end(), __k); }
	const_iterator upper_bound(const key_type& __k) const
	{ return _M_upper_bound(_M_begin(), _M_end(), __k); }

	pair<iterator, iterator> equal_range(cons t key_type& __k);
	pair<const_iterator, const_iterator> equal_range(const key_type& __k) const;

	friend bool operator==(const _Rb_tree& __x, const _Rb_tree& __y)
    {
		return __x.size() == __y.size() 
		&& std::equal(__x.begin(), __x.end(), __y.begin());
	}
    friend bool operator<(const _Rb_tree& __x, const _Rb_tree& __y)
    { return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end()); }
};

template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc>
void swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x, 
			_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
{ __x.swap(__y); }





}
#endif