#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <memory>
#include "VectorBase.hpp"
#include <algorithm>
#include <limits>
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
    typedef T                                      value_type;
    typedef Allocator                               allocator_type;
//    typedef typename base::alloc_traits          alloc_traits;
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

	//생성자
	explicit Vector(const allocator_type& a) : base(a)
	{}
	explicit Vector(size_type n, const value_type& x, const allocator_type& a);
	template <class InputIterator>
	Vector(InputIterator first, InputIterator last, const allocator_type& a);
	Vector(InputIterator first, InputIterator last, const allocator_type& a);
	Vector(const Vector& copy);
	~Vector(){}
	Vector& operator=(const Vector& ref);
private :
	//utils
	void vallocate(size_type n);
	void vdeallocate();
	void construct_at_end(size_type n);
	void construct_at_end(size_type n, const_reference val);
	template <class InputIterator>
	void construct_at_end(InputIterator first, InputIterator second, size_type n);
	size_type recommend(size_type new_size) const;
	void append(size_type n);
    void append(size_type n, const_reference x);
	//Member functions
	//iterator
	//Capacity
	size_type size() const
        {return static_cast<size_type>(this->end - this->begin);}
    size_type capacity() const 
        {return base::capacity();}
    bool empty() const
    	{return this->begin == this->end;}
	size_type max_size() const;
	void resize(size_type sz);
    void resize(size_type sz, const_reference x);
	//Element access:
	//Modifiers:
	void assign(size_type n, const_reference u);
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);
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
	return (std::min<size_type>(allocator_type::max_size(), 
								std::numeric_limits<difference_type>::max()));
}
template <class T, class Allocator>
void Vector<T, Allocator>::vallocate(size_type n)
{
	if (n > max_size())
		this->throwLengthError();
	this->begin = this->end = allocator_type::allocate(n, const_pointer);
	this->end_cap() = this->begin + n;
}
template <class T, class Allocator>
void Vector<T, Allocator>::vdeallocate()
{
	if (this->begin != nullptr)
	{
		clear();
		allocator_type::deallocate(this->begin, capacity());
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
template <class T, class Allocator>
template <class InputIterator>
void Vector<T, Allocator>::construct_at_end(InputIterator first, InputIterator second, size_type n)
{
	ConstructTransaction tx(*this, n);
	for (;first != second; ++first ,++tx.pos)
	{
		Allocator::construct(tx.pos, *first);
	}
}
template <class T, class Allocator>
typename Vector<T, Allocator>::size_type 
Vector<T, Allocator>::recommend(size_type new_size) const
{
	const size_type ms = max_size();
	if (new_size > ms)
		this->throwLengthError();
	const size_type capa = capacity();
	if (capa >= ms / 2)
		return (ms);
	return std::min<size_type>(2 * capa, new_size);
}
template <class T, class Allocator>
void Vector<T, Allocator>::append(size_type n)
{
	if (static_cast<size_type>(this->end_cap() - this->end) >= n)
		this->construct_at_end(n);
	else
	{

	}
}
template <class T, class Allocator>
void Vector<T, Allocator>::append(size_type n, const_reference x)
{
	if (static_cast<size_type>(this->end_cap() - this->end) >= n)
		this->construct_at_end(n, x);
	else
	{
		
	}
}

//Member functions
template <class T, class Allocator>
Vector<T, Allocator>::Vector(size_type n, const value_type& x, const allocator_type& a) : base(a)
{
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(n,x);
	}
}
template <class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& copy) : base(copy.alloc())
{//값 복사 해야하는거 아닌가? 그냥 대입연산자 호출할까..
	size_type n = copy.size();
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(n);
	}
}
template <class T, class Allocator>
template <class InputIterator>
Vector<T, Allocator>::Vector(InputIterator first, InputIterator last, const allocator_type& a) : base(a)
{
	size_type n = static_cast<size_type>(std::distance(first, last));
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(first, last, n);
	}
}
template <class T, class Allocator>
template <class InputIterator>
Vector<T, Allocator>::Vector(InputIterator first, InputIterator last, const allocator_type& a)
{
	size_type n = static_cast<size_type>(std::distance(first, last));
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(first, last, n);
	}
}
template <class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& ref)
{
	if (this != &ref)
	{
		base::copy_assign_alloc(ref);
		//assign()
	}
	return (*this);
}

//Modifiers:
template <class T, class Allocator>
void Vector<T, Allocator>::assign(size_type n, const_reference u)
{
	if (n <= capacity())
	{
		size_type s = size();
		std::fill_n(this->begin, std::min(n,s), u);
		if (n > s)
			this->construct_at_end(n - s, u);
		else
			this->destruct_at_end(this->begin + n);
	}
	else
	{
		vdeallocate();
		vallocate(recommend(n));
		construct_at_end(n, u);
	}
}
template <class T, class Allocator>
template <class InputIterator>
void Vector<T, Allocator>::assign(InputIterator first, InputIterator last)
{
	size_type new_size = static_cast<size_type>(std::distance(first, last));
	if (new_size <= capacity())
	{
		InputIterator mid = last;
		bool growing = false;
		if (new_size > size())
		{
			growing = true;
			mid = first;
			std::advance(mid, size());
		}
		pointer m = std::copy(first, mid, this->begin);
		if (growing)
			construct_at_end(mid, last, new_size - size());
		else
			this->destruct_at_end(m);
	}
	else
	{
		vdeallocate();
		vallocate(recommend(new_size));
		construct_at_end(first, last, new_size);
	}
}
//Capacity
template <class T, class Allocator>
void Vector<T, Allocator>::resize(size_type sz)
{
	size_type cs = size();
/*	if (cs < sz)
		this->append(sz - cs);
	else*/
		this->destruct_at_end(this->begin + sz);
}
template <class T, class Allocator>
void Vector<T, Allocator>::resize(size_type sz, const_reference x)
{
	size_type cs = size();
/*	if (cs < sz)
		this->append(sz - cs, x);
	else*/
		this->destruct_at_end(this->begin + sz);
}
};

#endif