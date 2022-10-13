#ifndef TREE_HPP
#define TREE_HPP

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

#endif