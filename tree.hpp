#ifndef TREE_HPP
#define TREE_HPP

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
_NodePtr
tree_min(_NodePtr x)
{
    while (x->left_ != nullptr)
        x = x->left_;
    return x;
}

template <class _NodePtr>
_NodePtr
tree_max(_NodePtr x)
{
    while (x->right_ != nullptr)
        x = x->right_;
    return x;
}
#endif