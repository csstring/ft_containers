
#include "algorithm"
namespace ft
{
struct _Rb_tree_node_base
{
	typedef _Rb_tree_node_base* _Base_ptr;
	typedef const _Rb_tree_node_base* _Const_Base_ptr;
	_Base_ptr		_M_parent;
	_Base_ptr		_M_left;
	_Base_ptr		_M_right;
	bool	_is_Black;
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
static _Rb_tree_node_base*
local_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
{
    if (__x->_M_right != 0)
    {
    	__x = __x->_M_right;
        while (__x->_M_left != 0)
        	__x = __x->_M_left;
    }
    else
    {
        _Rb_tree_node_base* __y = __x->_M_parent;
        while (__x == __y->_M_right)
        {
        	__x = __y;
        	__y = __y->_M_parent;
        }
        if (__x->_M_right != __y)
        	__x = __y;
    }
	return __x;
}

_Rb_tree_node_base*
_Rb_tree_increment(_Rb_tree_node_base* __x) throw ()
{
	return local_Rb_tree_increment(__x);
}

const _Rb_tree_node_base*
_Rb_tree_increment(const _Rb_tree_node_base* __x) throw ()
{
    return local_Rb_tree_increment(const_cast<_Rb_tree_node_base*>(__x));
}

static _Rb_tree_node_base*
local_Rb_tree_decrement(_Rb_tree_node_base* __x) throw ()
{
    if (__x->_is_Black == false && __x->_M_parent->_M_parent == __x)
    	__x = __x->_M_right;
    else if (__x->_M_left != 0)
    {
        _Rb_tree_node_base* __y = __x->_M_left;
        while (__y->_M_right != 0)
          __y = __y->_M_right;
        __x = __y;
    }
    else
    {
        _Rb_tree_node_base* __y = __x->_M_parent;
        while (__x == __y->_M_left)
        {
        	__x = __y;
            __y = __y->_M_parent;
        }
        __x = __y;
    }
    return __x;
}

_Rb_tree_node_base*
_Rb_tree_decrement(_Rb_tree_node_base* __x) throw ()
{
    return local_Rb_tree_decrement(__x);
}

const _Rb_tree_node_base*
_Rb_tree_decrement(const _Rb_tree_node_base* __x) throw ()
{
    return local_Rb_tree_decrement(const_cast<_Rb_tree_node_base*>(__x));
}

static void 
local_Rb_tree_rotate_left(_Rb_tree_node_base* const x,
		             		_Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* const y = x->_M_right;

    x->_M_right = y->_M_left;
    if (y->_M_left !=0)
    	y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
    	root = y;
    else if (x == x->_M_parent->_M_left)
    	x->_M_parent->_M_left = y;
    else
    	x->_M_parent->_M_right = y;
    y->_M_left = x;
    x->_M_parent = y;
}


static void
local_Rb_tree_rotate_right(_Rb_tree_node_base* const x,
			     _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* const y = x->_M_left;

    x->_M_left = y->_M_right;
    if (y->_M_right != 0)
      y->_M_right->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
      root = y;
    else if (x == x->_M_parent->_M_right)
      x->_M_parent->_M_right = y;
    else
      x->_M_parent->_M_left = y;
    y->_M_right = x;
    x->_M_parent = y;
}

void
_Rb_tree_insert_and_rebalance(const bool	__insert_left,
                                _Rb_tree_node_base* __x,
                                _Rb_tree_node_base* __p,
                                _Rb_tree_node_base& __header) throw ()
{
    _Rb_tree_node_base *& __root = __header._M_parent;

    __x->_M_parent = __p;
    __x->_M_left = 0;
    __x->_M_right = 0;
    __x->_is_Black = false;

    // Insert.
    // N.B. First node is always inserted left.
    if (__insert_left)
    {
    	__p->_M_left = __x;
        if (__p == &__header) //__p==root, __x is frist
        {
            __header._M_parent = __x;
            __header._M_right = __x;
        }
        else if (__p == __header._M_left)
        	__header._M_left = __x; // maintain leftmost pointing to min node
    }
    else
    {
        __p->_M_right = __x;
        if (__p == __header._M_right)
        	__header._M_right = __x; // maintain rightmost pointing to max node
    }
    // Rebalance.
    while (__x != __root && __x->_M_parent->_is_Black == false)
    {
		_Rb_tree_node_base* const __xpp = __x->_M_parent->_M_parent;

		if (__x->_M_parent == __xpp->_M_left)
	  	{
	    	_Rb_tree_node_base* const __y = __xpp->_M_right;
	    	if (__y && __y->_is_Black == false)//recursive case1,2,3
	    	{
				__x->_M_parent->_is_Black = true;
				__y->_is_Black = true;
				__xpp->_is_Black = false;
				__x = __xpp;
	    	}
	    	else
	    	{
				if (__x == __x->_M_parent->_M_right) //case4
				{
		   			__x = __x->_M_parent;
		    		local_Rb_tree_rotate_left(__x, __root);
				}
				//case 5
				__x->_M_parent->_is_Black = true;
				__xpp->_is_Black = false;
				local_Rb_tree_rotate_right(__xpp, __root);
	    	}
		}
		else
		{
			_Rb_tree_node_base* const __y = __xpp->_M_left;
	    	if (__y && __y->_is_Black == false)
	    	{
				__x->_M_parent->_is_Black = true;
				__y->_is_Black = true;
				__xpp->_is_Black = false;
				__x = __xpp;
	    	}
	    	else
	    	{
				if (__x == __x->_M_parent->_M_left)
		  		{
		   			__x = __x->_M_parent;
		    		local_Rb_tree_rotate_right(__x, __root);
				}
				__x->_M_parent->_is_Black = true;
				__xpp->_is_Black = false;
				local_Rb_tree_rotate_left(__xpp, __root);
	    	}
		}
	}
	__root->_is_Black = true;
}

_Rb_tree_node_base* 
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
						       _Rb_tree_node_base& __header) throw ()
{
	_Rb_tree_node_base *& __root = __header._M_parent;
    _Rb_tree_node_base *& __leftmost = __header._M_left;
    _Rb_tree_node_base *& __rightmost = __header._M_right;
    _Rb_tree_node_base* __y = __z;
    _Rb_tree_node_base* __x = 0;
    _Rb_tree_node_base* __x_parent = 0;

    if (__y->_M_left == 0)     // __z has at most one non-null child. y == z.
    	__x = __y->_M_right;     // __x might be null.
    else
	{
    	if (__y->_M_right == 0)  // __z has exactly one non-null child. y == z.
			__x = __y->_M_left;    // __x is not null.
    	else
		{
	  	// __z has two non-null children.  Set __y to
			__y = __y->_M_right;   //   __z's successor.  __x might be null.
			while (__y->_M_left != 0)
	    		__y = __y->_M_left;
	  		__x = __y->_M_right;
		}
	}
    if (__y != __z)
    {
		//relink y,z
		__z->_M_left->_M_parent = __y;
		__y->_M_left = __z->_M_left;
		//x move to y
		if (__y != __z->_M_right)
		{
	    	__x_parent = __y->_M_parent;
	    	if (__x) 
				__x->_M_parent = __y->_M_parent;
	    	__y->_M_parent->_M_left = __x;   // __y must be a child of _M_left
	    	__y->_M_right = __z->_M_right;
	    	__z->_M_right->_M_parent = __y;
		}
		else
			__x_parent = __y;
		// y, z swap
		if (__root == __z)
			__root = __y;
		else if (__z->_M_parent->_M_left == __z)
			__z->_M_parent->_M_left = __y;
		else
			__z->_M_parent->_M_right = __y;
		__y->_M_parent = __z->_M_parent;
		std::swap(__y->_is_Black, __z->_is_Black);
		__y = __z;
    }
    else // __y == __z,(parent and x link, leftmost and rightmost exechange) 
    {
		__x_parent = __y->_M_parent;
		if (__x) //__x_ NULL check
			__x->_M_parent = __y->_M_parent;
		if (__root == __z)
			__root = __x;
		else //z_parent, x link
		{
	  		if (__z->_M_parent->_M_left == __z)
	    		__z->_M_parent->_M_left = __x;
			else
				__z->_M_parent->_M_right = __x;
		}
		if (__leftmost == __z)
		{
	    	if (__z->_M_right == 0)        // __z->_M_left must be null also
	    		__leftmost = __z->_M_parent;
	    	else
	    		__leftmost = _Rb_tree_node_base::_S_minimum(__x);
		}
		if (__rightmost == __z)
	  	{
	    	if (__z->_M_left == 0)         // __z->_M_right must be null also
	      		__rightmost = __z->_M_parent;
	    	else
	    		__rightmost = _Rb_tree_node_base::_S_maximum(__x);
	  	}
    }
//    if (__y->_is_Black != false)
	//reblence
	if (__y->_is_Black == true) //m is black, c is black or red
    {
		//c is black
		while (__x != __root && (__x == 0 || __x->_is_Black == true))
		{
			if (__x == __x_parent->_M_left)
	    	{
	      		_Rb_tree_node_base* __w = __x_parent->_M_right;
				//case2
	      		if (__w->_is_Black == false)
				{
		  			__w->_is_Black = true;
		  			__x_parent->_is_Black = false;
		  			local_Rb_tree_rotate_left(__x_parent, __root);
		  			__w = __x_parent->_M_right;
				}
				//case 3
	      		if ((__w->_M_left == 0 || __w->_M_left->_is_Black == true) 
					&& (__w->_M_right == 0 || __w->_M_right->_is_Black == true))
				{
		  			__w->_is_Black = false;
		  			__x = __x_parent;
		  			__x_parent = __x_parent->_M_parent;
				}
	      		else
				{
					//case5
		  			if (__w->_M_right == 0 || __w->_M_right->_is_Black == true)
		    		{
		    			__w->_M_left->_is_Black = true;
		      			__w->_is_Black = false;
		      			local_Rb_tree_rotate_right(__w, __root);
		      			__w = __x_parent->_M_right;
		    		}
					//case6
		  			__w->_is_Black = __x_parent->_is_Black;
		  			__x_parent->_is_Black = true;
		  			if (__w->_M_right)
		    			__w->_M_right->_is_Black = true;
		  			local_Rb_tree_rotate_left(__x_parent, __root);
		  			break;
				}
	    	}
			else
	    	{
	      		// same as above, with _M_right <-> _M_left.
	    		_Rb_tree_node_base* __w = __x_parent->_M_left;
	    		if (__w->_is_Black == false)
				{
					__w->_is_Black = true;
		  			__x_parent->_is_Black = false;
		  			local_Rb_tree_rotate_right(__x_parent, __root);
		  			__w = __x_parent->_M_left;
				}
	      		if ((__w->_M_right == 0 || __w->_M_right->_is_Black == true) &&
		  				(__w->_M_left == 0 || __w->_M_left->_is_Black == true))
				{
		  			__w->_is_Black = false;
		  			__x = __x_parent;
		  			__x_parent = __x_parent->_M_parent;
				}
	      		else
				{
		  			if (__w->_M_left == 0 || __w->_M_left->_is_Black == true)
		    		{
		      			__w->_M_right->_is_Black = true;
		      			__w->_is_Black = false;
		      			local_Rb_tree_rotate_left(__w, __root);
		      			__w = __x_parent->_M_left;
		    		}
		  			__w->_is_Black = __x_parent->_is_Black;
		  			__x_parent->_is_Black = true;
		  			if (__w->_M_left)
		    			__w->_M_left->_is_Black = true;
		  			local_Rb_tree_rotate_right(__x_parent, __root);
		  		break;
				}
	    	}
		}
		// c is red
		if (__x) 
			__x->_is_Black = true;
    }
    return __y;
}

struct _Rb_tree_header
{
	_Rb_tree_node_base	_M_header;
	size_t		_M_node_count; // Keeps track of size of tree.

	_Rb_tree_header()
	{
		_M_header._is_Black = false;
		_M_reset();
	}
	void _M_move_data(_Rb_tree_header& from)
	{
		_M_header._is_Black = from._M_header._is_Black;
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
   
}
