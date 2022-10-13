#ifndef TREE_HPP
#define TREE_HPP
#include "utility.hpp"
#include <memory>

namespace ft
{

template <class _NodePtr>
bool tree_is_left_child(_NodePtr x)
{
    return x == x->parent_->left_;
}

template <class _NodePtr>
unsigned tree_sub_invariant(_NodePtr x)
{
    if (x == nullptr)
        return 1;
    if (x->left_ != nullptr && x->left_->parent_ != x)
        return 0;
    if (x->right_ != nullptr && x->right_->parent_ != x)
        return 0;
    if (x->left_ == x->right_ && x->left_ != nullptr)
        return 0;
    if (!x->is_black_)
    {
        if (x->left_ && !x->left_->is_black_)
            return 0;
        if (x->right_ && !x->right_->is_black_)
            return 0;
    }
    unsigned h = tree_sub_invariant(x->left_);
    if (h == 0)
        return 0;
    if (h != tree_sub_invariant(x->right_))
        return 0;
    return h + x->is_black_;
}

template <class _NodePtr>
bool tree_invariant(_NodePtr root)
{
    if (root == nullptr)
        return true;
    if (root->parent_ == nullptr)
        return false;
    if (!tree_is_left_child(root))
        return false;
    if (!root->is_black_)
        return false;
    return tree_sub_invariant(root) != 0;
}

template <class _NodePtr>
_NodePtr tree_min(_NodePtr x)
{
    while (x->left_ != nullptr)
        x = x->left_;
    return x;
}

template <class _NodePtr>
_NodePtr tree_max(_NodePtr x)
{
    while (x->right_ != nullptr)
        x = x->right_;
    return x;
}
template <class _EndNodePtr, class _NodePtr>
_EndNodePtr tree_next_iter(_NodePtr x)
{
    if (x->right_ != nullptr)
        return static_cast<_EndNodePtr>(tree_min(x->right_));
    while (!tree_is_left_child(x))
        x = x->parent_unsafe();
    return static_cast<_EndNodePtr>(x->parent_);
}

template <class _NodePtr, class _EndNodePtr>
_NodePtr tree_prev_iter(_EndNodePtr x)
{
    if (x->left_ != nullptr)
        return tree_max(x->left_);
    _NodePtr xx = static_cast<_NodePtr>(x);
    while (tree_is_left_child(xx))
        xx = xx->parent_unsafe();
    return xx->parent_unsafe();
}

template <class _NodePtr>
_NodePtr tree_leaf(_NodePtr x)
{
    while (true)
    {
        if (x->left_ != nullptr)
        {
            x = x->left_;
            continue;
        }
        if (x->right_ != nullptr)
        {
            x = x->right_;
            continue;
        }
        break;
    }
    return x;
}

template <class _NodePtr>
void tree_left_rotate(_NodePtr x)
{
    _NodePtr y = x->right_;
    x->right_ = y->left_;
    if (x->right_ != nullptr)
        x->right_->set_parent(x);
    y->parent_ = x->parent_;
    if (tree_is_left_child(x))
        x->parent_->left_ = y;
    else
        x->parent_unsafe()->right_ = y;
    y->left_ = x;
    x->set_parent(y);
}

template <class _NodePtr>
void tree_right_rotate(_NodePtr x)
{
    _NodePtr y = x->left_;
    x->left_ = y->right_;
    if (x->left_ != nullptr)
        x->left_->set_parent(x);
    y->parent_ = x->parent_;
    if (tree_is_left_child(x))
        x->parent_->left_ = y;
    else
        x->parent_unsafe()->right_ = y;
    y->right_ = x;
    x->set_parent(y);
}

template <class _NodePtr>
void tree_balance_after_insert(_NodePtr root, _NodePtr x)
{
    x->is_black_ = x == root;
    while (x != root && !x->parent_unsafe()->is_black_)
    {
        if (tree_is_left_child(x->parent_unsafe()))
        {
            _NodePtr y = x->parent_unsafe()->parent_unsafe()->right_;
            if (y != nullptr && !y->is_black_)
            {
                x = x->parent_unsafe();
                x->is_black_ = true;
                x = x->parent_unsafe();
                x->is_black_ = x == root;
                y->is_black_ = true;
            }
            else
            {
                if (!tree_is_left_child(x))
                {
                    x = x->parent_unsafe();
                    tree_left_rotate(x);
                }
                x = x->parent_unsafe();
                x->is_black_ = true;
                x = x->parent_unsafe();
                x->is_black_ = false;
                tree_right_rotate(x);
                break;
            }
        }
        else
        {
            _NodePtr y = x->parent_unsafe()->parent_->left_;
            if (y != nullptr && !y->is_black_)
            {
                x = x->parent_unsafe();
                x->is_black_ = true;
                x = x->parent_unsafe();
                x->is_black_ = x == root;
                y->is_black_ = true;
            }
            else
            {
                if (tree_is_left_child(x))
                {
                    x = x->parent_unsafe();
                    tree_right_rotate(x);
                }
                x = x->parent_unsafe();
                x->is_black_ = true;
                x = x->parent_unsafe();
                x->is_black_ = false;
                tree_left_rotate(x);
                break;
            }
        }
    }
}

template <class _NodePtr>
void tree_remove(_NodePtr root, _NodePtr z)
{
    // z will be removed from the tree.  Client still needs to destruct/deallocate it
    // y is either z, or if z has two children, tree_next(z).
    // y will have at most one child.
    // y will be the initial hole in the tree (make the hole at a leaf)
    _NodePtr y = (z->left_ == nullptr || z->right_ == nullptr) ?
                    z : tree_next(z);
    // x is y's possibly null single child
    _NodePtr x = y->left_ != nullptr ? y->left_ : y->right_;
    // w is x's possibly null uncle (will become x's sibling)
    _NodePtr w = nullptr;
    // link x to y's parent, and find w
    if (x != nullptr)
        x->parent_ = y->parent_;
    if (tree_is_left_child(y))
    {
        y->parent_->left_ = x;
        if (y != root)
            w = y->parent_unsafe()->right_;
        else
            root = x;  // w == nullptr
    }
    else
    {
        y->parent_unsafe()->right_ = x;
        // y can't be root if it is a right child
        w = y->parent_->left_;
    }
    bool removed_black = y->is_black_;
    // If we didn't remove z, do so now by splicing in y for z,
    //    but copy z's color.  This does not impact x or w.
    if (y != z)
    {
        // z->left_ != nulptr but z->right_ might == x == nullptr
        y->parent_ = z->parent_;
        if (tree_is_left_child(z))
            y->parent_->left_ = y;
        else
            y->parent_unsafe()->right_ = y;
        y->left_ = z->left_;
        y->left_->set_parent(y);
        y->right_ = z->right_;
        if (y->right_ != nullptr)
            y->right_->set_parent(y);
        y->is_black_ = z->is_black_;
        if (root == z)
            root = y;
    }
    // There is no need to rebalance if we removed a red, or if we removed
    //     the last node.
    if (removed_black && root != nullptr)
    {
        // Rebalance:
        // x has an implicit black color (transferred from the removed y)
        //    associated with it, no matter what its color is.
        // If x is root (in which case it can't be null), it is supposed
        //    to be black anyway, and if it is doubly black, then the double
        //    can just be ignored.
        // If x is red (in which case it can't be null), then it can absorb
        //    the implicit black just by setting its color to black.
        // Since y was black and only had one child (which x points to), x
        //   is either red with no children, else null, otherwise y would have
        //   different black heights under left and right pointers.
        // if (x == root || x != nullptr && !x->is_black_)
        if (x != nullptr)
            x->is_black_ = true;
        else
        {
            //  Else x isn't root, and is "doubly black", even though it may
            //     be null.  w can not be null here, else the parent would
            //     see a black height >= 2 on the x side and a black height
            //     of 1 on the w side (w must be a non-null black or a red
            //     with a non-null black child).
            while (true)
            {
                if (!tree_is_left_child(w))  // if x is left child
                {
                    if (!w->is_black_)
                    {
                        w->is_black_ = true;
                        w->parent_unsafe()->is_black_ = false;
                        tree_left_rotate(w->parent_unsafe());
                        // x is still valid
                        // reset root only if necessary
                        if (root == w->left_)
                            root = w;
                        // reset sibling, and it still can't be null
                        w = w->left_->right_;
                    }
                    // w->is_black_ is now true, w may have null children
                    if ((w->left_  == nullptr || w->left_->is_black_) &&
                        (w->right_ == nullptr || w->right_->is_black_))
                    {
                        w->is_black_ = false;
                        x = w->parent_unsafe();
                        // x can no longer be null
                        if (x == root || !x->is_black_)
                        {
                            x->is_black_ = true;
                            break;
                        }
                        // reset sibling, and it still can't be null
                        w = tree_is_left_child(x) ?
                                    x->parent_unsafe()->right_ :
                                    x->parent_->left_;
                        // continue;
                    }
                    else  // w has a red child
                    {
                        if (w->right_ == nullptr || w->right_->is_black_)
                        {
                            // w left child is non-null and red
                            w->left_->is_black_ = true;
                            w->is_black_ = false;
                            tree_right_rotate(w);
                            // w is known not to be root, so root hasn't changed
                            // reset sibling, and it still can't be null
                            w = w->parent_unsafe();
                        }
                        // w has a right red child, left child may be null
                        w->is_black_ = w->parent_unsafe()->is_black_;
                        w->parent_unsafe()->is_black_ = true;
                        w->right_->is_black_ = true;
                        tree_left_rotate(w->parent_unsafe());
                        break;
                    }
                }
                else
                {
                    if (!w->is_black_)
                    {
                        w->is_black_ = true;
                        w->parent_unsafe()->is_black_ = false;
                        tree_right_rotate(w->parent_unsafe());
                        // x is still valid
                        // reset root only if necessary
                        if (root == w->right_)
                            root = w;
                        // reset sibling, and it still can't be null
                        w = w->right_->left_;
                    }
                    // w->is_black_ is now true, w may have null children
                    if ((w->left_  == nullptr || w->left_->is_black_) &&
                        (w->right_ == nullptr || w->right_->is_black_))
                    {
                        w->is_black_ = false;
                        x = w->parent_unsafe();
                        // x can no longer be null
                        if (!x->is_black_ || x == root)
                        {
                            x->is_black_ = true;
                            break;
                        }
                        // reset sibling, and it still can't be null
                        w = tree_is_left_child(x) ?
                                    x->parent_unsafe()->right_ :
                                    x->parent_->left_;
                        // continue;
                    }
                    else  // w has a red child
                    {
                        if (w->left_ == nullptr || w->left_->is_black_)
                        {
                            // w right child is non-null and red
                            w->right_->is_black_ = true;
                            w->is_black_ = false;
                            tree_left_rotate(w);
                            // w is known not to be root, so root hasn't changed
                            // reset sibling, and it still can't be null
                            w = w->parent_unsafe();
                        }
                        // w has a left red child, right child may be null
                        w->is_black_ = w->parent_unsafe()->is_black_;
                        w->parent_unsafe()->is_black_ = true;
                        w->left_->is_black_ = true;
                        tree_right_rotate(w->parent_unsafe());
                        break;
                    }
                }
            }
        }
    }
}

template <class _Tp>
struct tree_key_value_types 
{
  typedef _Tp key_type;
  typedef _Tp node_value_type;
  typedef _Tp container_value_type;
  static const bool is_map = false;

  static key_type const& get_key(_Tp const& v) {
    return v;
  }
  static container_value_type const& get_value(node_value_type const& v) {
    return v;
  }
  static container_value_type* get_ptr(node_value_type& n) {
    return n;
  }
};

template <class _Key, class _Tp>
struct tree_key_value_types<value_type<_Key, _Tp> > //value_type map에서 추가
{
	typedef _Key                                         key_type;
	typedef _Tp                                          mapped_type;
	typedef value_type<_Key, _Tp>                      node_value_type;
	typedef pair<const _Key, _Tp>                        container_value_type;
	typedef container_value_type                       map_value_type;
	static const bool is_map = true;

	static key_type const& get_key(node_value_type const& t)
	{ return t.get_value().first; }
	
	template <class _Up> 
	static typename ft::enable_if<is_same_uncvref<_Up, container_value_type>::value, key_type const&>::type
	get_key(_Up& t) { return t.first; }
	static container_value_type const& get_value(node_value_type const& t)
	{ return t.get_value(); }

	template <class _Up>
	static typename ft::enable_if<is_same_uncvref<_Up, container_value_type>::value,
		container_value_type const&>::type
		get_value(_Up& t) { return t; }
	
	static container_value_type* get_ptr(node_value_type& n) { return n.get_value();}
};

template <class _VoidPtr>
struct tree_node_base_types
{
  typedef _VoidPtr                                               void_pointer;

  typedef tree_node_base<void_pointer>                      node_base_type;
  typedef typename node_base_type* node_base_pointer;

  typedef tree_end_node<node_base_pointer>                  end_node_type;
  typedef typename end_node_type* end_node_pointer;
  typedef end_node_pointer parent_pointer;
};

template <class _Tp, class _AllocPtr, class _KVTypes = tree_key_value_types<_Tp>, bool = _KVTypes::is_map>
struct tree_map_pointer_types {};

template <class _Tp, class _AllocPtr, class _KVTypes>
struct tree_map_pointer_types<_Tp, _AllocPtr, _KVTypes, true> 
{
  typedef typename _KVTypes::map_value_type   _Mv;
  typedef typename _Mv* map_value_type_pointer;
  typedef typename const _Mv* const_map_value_type_pointer;
};

template <class _NodePtr, class _NodeT>
struct tree_node_types;

template <class _NodePtr, class _Tp, class _VoidPtr>
struct tree_node_types<_NodePtr, tree_node<_Tp, _VoidPtr> >
    : public tree_node_base_types<_VoidPtr>,
             tree_key_value_types<_Tp>,
             tree_map_pointer_types<_Tp, _VoidPtr>
{
  typedef tree_node_base_types<_VoidPtr> base;
  typedef tree_key_value_types<_Tp>      key_base;
  typedef tree_map_pointer_types<_Tp, _VoidPtr> map_pointer_base;
public:

 //typedef typename pointer_traits<_NodePtr>::element_type       node_type;
  typedef _NodePtr                                              node_pointer;

  typedef _Tp                                                 node_value_type;
  typedef typename node_value_type* node_value_type_pointer;
  typedef typename const node_value_type* const_node_value_type_pointer;
  typedef typename base::end_node_pointer iter_pointer;
};

//node
template <class _Pointer>
class tree_end_node
{
public:
    typedef _Pointer pointer;
    pointer left_;

    tree_end_node() : left_() {}
};

template <class _VoidPtr>
class tree_node_base: public tree_node_base_types<_VoidPtr>::end_node_type
{
	typedef tree_node_base_types<_VoidPtr> _NodeBaseTypes;

public:
	typedef typename _NodeBaseTypes::node_base_pointer pointer;
	typedef typename _NodeBaseTypes::parent_pointer parent_pointer;

	pointer	  right_;
	parent_pointer parent_;
	bool is_black_;

	pointer parent_unsafe() const { return static_cast<pointer>(parent_);}

	void set_parent(pointer p) 
	{ parent_ = static_cast<parent_pointer>(p);}

private:
  ~tree_node_base();
  tree_node_base(tree_node_base const&);
  tree_node_base& operator=(tree_node_base const&);
};

template <class _Tp, class _VoidPtr>
class tree_node : public tree_node_base<_VoidPtr>
{
public:
	typedef _Tp node_value_type;
	node_value_type value_;

private:
  ~tree_node();
  tree_node(tree_node const&);
  tree_node& operator=(tree_node const&);
};

template <class _Allocator>
class tree_node_destructor
{
    typedef _Allocator                                      allocator_type;
    typedef std::allocator<allocator_type>                alloc;

public:
    typedef typename alloc::pointer                pointer;
private:
    typedef tree_node_types<pointer> _NodeTypes;
    allocator_type& na_;
	tree_node_destructor& operator=(const tree_node_destructor&);

public:
    bool value_constructed;

    explicit tree_node_destructor(allocator_type& na, bool val = false)
        : na_(na), value_constructed(val) {}
    void operator()(pointer p)
    {
        if (value_constructed)
            alloc::destroy(_NodeTypes::get_ptr(p->value_));
        if (p)
            alloc::deallocate(p, 1);
    }
    template <class> friend class map_node_destructor;
};


}
#endif