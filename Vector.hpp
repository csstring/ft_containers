#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <memory>
#include "VectorBase.hpp"
 //나중에 삭제
#include <iterator> //나중에 직접만든걸로 수저
namespace ft
{
template<class T, class Allocator = std::allocator<T> >
class Vector : private VectorBase<T, Allocator>
{
private:
	typedef VectorBase<T, Allocator>	base;
    typedef std::allocator<T>                       defaultallocatortype;
public:
    typedef Vector                                   self;
    typedef T                                      valuetype;
    typedef Allocator                               allocatortype;
    typedef typename base::alloc_traits          alloc_traits;
    typedef typename base::reference               reference;
    typedef typename base::const_reference         const_reference;
    typedef typename base::size_type               size_type;
    typedef typename base::difference_type         difference_type;
    typedef typename base::pointer                 pointer;
    typedef typename base::const_pointer           const_pointer;
	//아래 4개는 다시 만들어야함
    typedef std::__wrap_iter<pointer>                     iterator;
    typedef std::__wrap_iter<const_pointer>               const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
private :
	//utils
	size_type max_size() const;
	void vallocate(size_type n);
	void vdeallocate();
	void construct_at_end(size_type n);
	void construct_at_end(size_type n, const_reference val);
	//Member functions
	//iterator
	//Capacity
	//Element access:
	//Modifiers:
	//Allocator:
	//Non-member function overloads
	struct ConstructTransaction 
	{
    explicit ConstructTransaction(vector &ref, size_type n)
      : CTV(ref),  pos(ref.end), new_end(ref.end + n) {}
    ~ConstructTransaction() {
      CTV.end = pos;
    }
    Vector &CTV;
    pointer pos;
    const_pointer const new_end;
	};
};

//utils
template <class T, class Allocator>
Vector<T, Allocator>::size_type Vector<T, Allocator>::max_size() const
{
	return Allocator::max_size();
}
template <class T, class Allocator>
void Vector<T, Allocator>::vallocate(size_type n)
{
	if (n > max_size())
		this->throwLengthError();
	this->begin = this->end = alloc_traits::allocate(this->alloc(), n);
	this->end_cap() = this->begin + n;
}
template <class T, class Allocator>
void Vector<T, Allocator>::vdeallocate()
{
	if (this->begin != nullptr)
	{
		clear();
		alloc_traits::deallocate(this->alloc(), this->begin, capacity());
		this->begin = this->end = this->end_cap() = nullptr;
	}
}
template <class T, class Allocator>
void Vector<T, Allocator>::construct_at_end(size_type n)
{
	ConstructTransaction tx(*this, n);
	for (; tx.pos != tx.new_end; ++tx.pos)
	{
		Allocator::construct(tx.pos);
	}
}
template <class T, class Allocator>
void Vector<T, Allocator>::construct_at_end(size_type n, const_reference val)
{
	ConstructTransaction tx(*this, n);
	for (; tx.pos != tx.new_end; ++tx.pos)
	{
		Allocator::construct(tx.pos, val);
	}
}
};

#endif