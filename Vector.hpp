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
template <class T, class Allocator = std::allocator<T> >
class Vector : private VectorBase <T, Allocator>
{
private:
	typedef VectorBase<T, Allocator>	base;
    typedef Allocator                       default_allocator_type;
public:
    typedef Vector                                   self;
    typedef T                                      value_type;
    typedef Allocator                               allocator_type;
//  typedef typename base::alloc_traits          alloc_traits;
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
	explicit Vector(const allocator_type& a);
	explicit Vector(size_type n, const value_type& x, const allocator_type& a);
	template <class InputIterator>
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
	const_iterator make_iter(const_pointer __p); //이놈 왜 있는건지  모르겟음
	iterator               begin();
    const_iterator         begin()	const;
    iterator               end();
    const_iterator         end()	const;

    reverse_iterator	rbegin()
		{return	reverse_iterator(end());}
    
    const_reverse_iterator rbegin()  const
		{return const_reverse_iterator(end());}
    
    reverse_iterator	rend()
		{return	reverse_iterator(begin());}
    
    const_reverse_iterator rend()    const
		{return const_reverse_iterator(begin());}
	//Capacity
	size_type size() const
	 {return static_cast<size_type>(this->end - this->begin);}
    size_type capacity() const 
        {return base::capacity();}
    bool empty() const
    	{return this->begin == this->end;}
	void reserve(size_type __n);
	size_type max_size() const;
	void resize(size_type sz);
    void resize(size_type sz, const_reference x);
	//Element access:
	reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference       at(size_type n);
    const_reference at(size_type n) const;

	//_LIBCPP_ASSERT(!empty(), "front() called for empty vector"); assert까지 구현할 필요 있나?
    reference       front() { return *this->begin; }
    const_reference front() const { return *this->begin; }
    reference       back() { return *(this->end - 1); }
    const_reference back()  const { return *(this->end - 1); }
	//Modifiers:
	void assign(size_type n, const_reference u);
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);
	void push_back(const_reference x);
	void pop_back();
/*	iterator insert(const_iterator position, const_reference x);
    iterator insert(const_iterator position, size_type n, const_reference x);
    template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);*/
	void move_range(pointer __from_s, pointer __from_e, pointer __to);
	template <class InputIterator, class OutputIterator>
	OutputIterator move(InputIterator first, InputIterator last, OutputIterator result);
	template <class InputIterator, class OutputIterator>
	OutputIterator move_backward(InputIterator first, InputIterator last, OutputIterator result);
	iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
	void swap(Vector&);
	//Allocator:
	//Non-member function overloads
	struct ConstructTransaction 
	{
    explicit ConstructTransaction(Vector &ref, size_type n)
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
typename Vector<T, Allocator>::size_type 
Vector<T, Allocator>::max_size() const
{
	return (std::min<size_type>(allocator_type::max_size(), 
								std::numeric_limits<difference_type>::max()));
}
template <class T, class Allocator>
void Vector<T, Allocator>::vallocate(size_type n)
{
	if (n > max_size())
		this->throwLengthError();
	this->begin = this->end = allocator_type::allocate(this->alloc(), n);
	this->end_cap() = this->begin + n;
}
template <class T, class Allocator>
void Vector<T, Allocator>::vdeallocate()
{
	if (this->begin != nullptr)
	{
		this->clear();
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
		allocator_type& a = this->alloc();
		Vector<T, Allocator> temp_vector(recommend(size() + n), (void)0, a);
		temp_vector->end = std::copy(this->begin, this->end, temp_vector->begin);
		temp_vector.construct_at_end(n);
		std::swap(this->begin, temp_vector->begin);
		std::swap(this->end, temp_vector->end);
		std::swap(this->end_cap(), this->end_cap());
	}
}
template <class T, class Allocator>
void Vector<T, Allocator>::append(size_type n, const_reference x)
{
	if (static_cast<size_type>(this->end_cap() - this->end) >= n)
		this->construct_at_end(n, x);
	else
	{
		allocator_type& a = this->alloc();
		Vector<T, Allocator> temp_vector(recommend(size() + n), x, a);
		temp_vector->end = std::copy(this->begin, this->end, temp_vector->begin);
		temp_vector.construct_at_end(n, x);
		std::swap(this->begin, temp_vector->begin);
		std::swap(this->end, temp_vector->end);
		std::swap(this->end_cap(), this->end_cap());
	}
}

//Member functions
template <class T, class Allocator>
Vector<T, Allocator>::Vector(const allocator_type& a)
: VectorBase<T, Allocator>(a) {}
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
/*
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
}*/
template <class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& ref)
{
	if (this != &ref)
	{
		base::copy_assign_alloc(ref);
		assign(ref.begin, ref.end);
	}
	return (*this);
}
//iterator
template <class T, class Allocator>
typename Vector<T, Allocator>::const_iterator
Vector<T, Allocator>::make_iter(const_pointer p)
{
	return const_iterator(p);
}
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator
Vector<T, Allocator>::begin()
{
	return (make_iter(this->begin));
}
template <class T, class Allocator>
typename Vector<T, Allocator>::const_iterator
Vector<T, Allocator>::begin()	const
{
	return (make_iter(this->begin));
}
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator
Vector<T, Allocator>::end()
{
	return (make_iter(this->end));
}
template <class T, class Allocator>
typename Vector<T, Allocator>::const_iterator
Vector<T, Allocator>::end()	const
{
	return (make_iter(this->end));
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

template <class T, class Allocator>
void Vector<T, Allocator>::push_back(const_reference x)
{
	this->append(1, x);
}

template <class T, class Allocator>
void Vector<T, Allocator>::pop_back()
{
	this->destruct_at_end(this->end - 1);
}
/*
template <class T, class Allocator>
Vector<T, Allocator>::iterator
Vector<T, Allocator>::insert(const_iterator position, const_reference x)
{
	pointer p = this->begin - (position - this->begin());
	if (this->end != this->end_cap())
	{
		if (p == this->end)
			this->construct_at_end(1, x);
		else
		{
			move_range(p, this->end, p + 1);
			const_pointer xr = &x;
			if (p <= xr && xr < this->end)
				++xr;
			*p = *xr;
		}
	}
	else
		this->append(1, x);
	return (make_iter(p));
}

template <class T, class Allocator>
Vector<T, Allocator>::iterator
Vector<T, Allocator>::insert(const_iterator position, size_type n, const_reference x)
{
	pointer p = this->begin - (position - this->begin());
	if (n > 0)
	{
		if (n <= static_cast<size_type>(this->end_cap() - this->end))
		{

		}
		else
			this->append(n, x);
	}
	return (make_iter(p));
}
template <class T, class Allocator>
template <class InputIterator>
Vector<T, Allocator>::iterator
Vector<T, Allocator>::insert(const_iterator position, InputIterator first, InputIterator last);
*/
template <class T, class Allocator>
template <class InputIterator, class OutputIterator>
OutputIterator Vector<T, Allocator>::move_backward
(InputIterator first, InputIterator last, OutputIterator result)
{
	for (; first != last; --last, --result)
		*result = last;
	return (result);
}

template <class T, class Allocator>
void Vector<T, Allocator>::move_range(pointer from_s, pointer from_e, pointer to)
{
	pointer old_last = this->end;
	difference_type n = old_last - to;
	pointer i = from_s + n;
	construct_at_end(from_e - i);
	move_backward(from_s, from_s + n , old_last);
}

template <class T, class Allocator>
template <class InputIterator, class OutputIterator>
OutputIterator Vector<T, Allocator>::move
(InputIterator first, InputIterator last, OutputIterator result)
{
	for (; first != last; ++first, ++result)
		*result = first;
	return (result);
}

template <class T, class Allocator>
typename Vector<T, Allocator>::iterator 
Vector<T, Allocator>::erase(const_iterator position)
{
	difference_type ps = position - this->begin();
	pointer p = this->begin + ps;
	this->destruct_at_end(this->move(p + 1, this->end, p));
	iterator r = make_iter(p);
	return (r);
}

template <class T, class Allocator>
typename Vector<T, Allocator>::iterator 
Vector<T, Allocator>::erase(const_iterator first, const_iterator last)
{
	pointer p = this->begin + (first - this->begin());
	if (first != last)
	{
		this->destruct_at_end(this->move(p + (last - first), this->end, p));
	}
	iterator r = make_iter(p);
	return (r);
}

template <class T, class Allocator>
void Vector<T, Allocator>::swap(Vector& x)
{
	std::swap(this->begin, x.begin);
	std::swap(this->end, x.end);
	std::swap(this->end_cap(), x.end_cap());
	std::swap(this->alloc(), x.alloc());
}
//Capacity
template <class T, class Allocator>
void Vector<T, Allocator>::resize(size_type sz)
{
	size_type cs = size();
	if (cs < sz)
		this->append(sz - cs);
	else
		this->destruct_at_end(this->begin + sz);
}
template <class T, class Allocator>
void Vector<T, Allocator>::resize(size_type sz, const_reference x)
{
	size_type cs = size();
	if (cs < sz)
		this->append(sz - cs, x);
	else
		this->destruct_at_end(this->begin + sz);
}
template <class T, class Allocator>
void  Vector<T, Allocator>::reserve(size_type n)
{
	const size_type ms = max_size();
	if (n > ms)
		this->throwLengthError();
	if (n > capacity())
	{
		allocator_type& a = this->alloc();
		//const value_type& v = typename T;
		Vector<T, Allocator> temp_vector(n, (void)0, a);
		temp_vector->end = std::copy(this->begin, this->end, temp_vector->begin);
		std::swap(this->begin, temp_vector->begin);
		std::swap(this->end, temp_vector->end);
		std::swap(this->end_cap(), this->end_cap());
	}
}

//Element access:
template <class T, class Allocator>
typename Vector<T, Allocator>::reference 
Vector<T, Allocator>::operator[](size_type n)
{
	return (this->begin[n]);
}
template <class T, class Allocator>
typename Vector<T, Allocator>::const_reference 
Vector<T, Allocator>::operator[](size_type n) const
{
	return (this->begin[n]);
}
template <class T, class Allocator>
typename Vector<T, Allocator>::reference 
Vector<T, Allocator>::at(size_type n)
{
	if (n >= size())
		this->throwOutOfRange();
	return (this->begin[n]);
}
template <class T, class Allocator>
typename Vector<T, Allocator>::const_reference 
Vector<T, Allocator>::at(size_type n) const
{
	if (n >= size())
		this->throwOutOfRange();
	return (this->begin[n]);
}
};
#endif