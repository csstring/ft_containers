#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <memory>
#include <algorithm>
#include <limits>
#include "iterator.hpp"
 //나중에 삭제
#include <iostream> 
namespace ft
{
class vectorBaseCommon
{
protected:
	vectorBaseCommon(){}
	void	throwLengthError() const
	{
		std::__throw_length_error("vector");
	}
	void	throwOutOfRange() const
	{
		std::__throw_out_of_range("vector");
	}
};

template <class T, class Allocator = std::allocator<T> >
class vectorBase : protected vectorBaseCommon
{
public:
    typedef Allocator                                allocator_type;
//    typedef std::allocator_traits<allocator_type>    alloc_traits;
    typedef typename Allocator::size_type      	 	size_type;
protected:
    typedef typename Allocator::value_type	          value_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;
    typedef typename Allocator::difference_type	 difference_type;
    typedef typename Allocator::pointer           pointer;
    typedef typename Allocator::const_pointer     const_pointer;
    typedef pointer                                  iterator;
    typedef const_pointer                            const_iterator;

    pointer                                         __begin_;
    pointer                                         __end_;
	ft::pair<pointer, allocator_type> end_cap_val;
	allocator_type& __alloc() {return end_cap_val.second;}
    const allocator_type& __alloc() const {return end_cap_val.second;}
    pointer& __end_cap() {return end_cap_val.first;}
    const pointer& __end_cap() const {return end_cap_val.first;}

	vectorBase();
	vectorBase(const allocator_type& a);
	~vectorBase();
	void clear() { destruct_at_end(__begin_); }
	size_type capacity() const
		{ return static_cast<size_type>(__end_cap() - __begin_); }
	void destruct_at_end(pointer new_last);
	void copy_assign_alloc(const vectorBase& c)
	{
		if (__alloc() != c.__alloc())
		{
			clear();
			__alloc().deallocate(__begin_, capacity());
			__begin_ = __end_ = __end_cap() = nullptr;
		}
		__alloc() = c.__alloc();
	}
private:
};

template <class T, class Allocator>
void	vectorBase<T, Allocator>::destruct_at_end(pointer new_last)
{
	pointer soon_to_be_end = __end_;
	while (new_last != soon_to_be_end)
		__alloc().destroy(--soon_to_be_end);
	__end_ = new_last;
}

template <class T, class Allocator>
vectorBase<T, Allocator>::vectorBase() 
	:	__begin_(nullptr), 
		__end_(nullptr), 
		end_cap_val(nullptr, std::__default_init_tag()){}

template <class T, class Allocator>
vectorBase<T, Allocator>::vectorBase(const allocator_type& a)
	:	__begin_(nullptr), 
		__end_(nullptr), 
		end_cap_val(nullptr, a){}

template <class T, class Allocator>
vectorBase<T, Allocator>::~vectorBase()
{
	if (__begin_ != nullptr)
	{
		//Allocator a = __alloc();
		clear();
		__alloc().deallocate(__begin_, capacity());
	}
}

template <class T, class Allocator = std::allocator<T> >
class vector : private vectorBase <T, Allocator>
{
private:
	typedef vectorBase<T, Allocator>	base;
    typedef Allocator                       default_allocator_type;
public:
    typedef vector                                   self;
    typedef T                                      value_type;
    typedef Allocator                               allocator_type;
//  typedef typename base::alloc_traits          alloc_traits;
    typedef typename base::reference               reference;
    typedef typename base::const_reference         const_reference;
    typedef typename base::size_type               size_type;
    typedef typename base::difference_type         difference_type;
    typedef typename base::pointer                 pointer;
    typedef typename base::const_pointer           const_pointer;
//	typedef pointer                                  iterator;
//   typedef const_pointer                            const_iterator;
	//아래 4개는 다시 만들어야함
//	typedef typename std::vector<T>::iterator					base_iter;
//	typedef typename std::vector<T>::const_iterator				base_const_iter;
/*    typedef  vector_iterator<base_iter>                    iterator;
    typedef  vector_iterator<base_const_iter>              const_iterator;
	typedef  _reverse_iterator<base_iter> reverse_iterator;
    typedef  _reverse_iterator<base_const_iter>   const_reverse_iterator;*/

	typedef __vector_iterator<pointer> iterator;
  	typedef __vector_iterator<const_pointer> const_iterator;
  	typedef _reverse_iterator<iterator> reverse_iterator;
 	typedef _reverse_iterator<const_iterator> const_reverse_iterator;
	//생성자
	explicit vector(const allocator_type& a = Allocator());
	explicit vector(size_type n, const value_type& x = value_type(), const allocator_type& a = Allocator());
	template <class InputIterator>
	vector(
		InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last, 
		const allocator_type& a = Allocator());
	vector(const vector& copy);
	~vector(){}
	vector& operator=(const vector& ref);
	//Member functions
	//iterator
	const_iterator make_iter(const_pointer p) const; //이놈 왜 있는건지  모르겟음
	iterator make_iter(pointer p);
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
	 {return static_cast<size_type>(this->__end_ - this->__begin_);}
    size_type capacity() const 
        {return base::capacity();}
    bool empty() const
    	{return this->__begin_ == this->__end_;}
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
    reference       front() { return *this->__begin_; }
    const_reference front() const { return *this->__begin_; }
    reference       back() { return *(this->__end_ - 1); }
    const_reference back()  const { return *(this->__end_ - 1); }
	//Allocator
	allocator_type get_allocator() const
	{ return (this->__alloc()); }
	//Modifiers:
	void assign(size_type n, const_reference u);
	template <class InputIterator>
	void assign(
		InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value && 
		is_same<typename iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value, InputIterator>::type last);
	template <class ForwardIterator>
	void assign( 
		ForwardIterator first, 
		typename ft::enable_if<!ft::is_integral<ForwardIterator>::value && 
		(is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::forward_iterator_tag>::value || 
		is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::bidirectional_iterator_tag>::value || 
		is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::random_access_iterator_tag>::value), ForwardIterator>::type last);
	void push_back(const_reference x);
	void pop_back();
	void swap(vector& x);
	void clear()
	{
		base::clear();	
	}
	iterator insert(const_iterator position, const_reference x);
    void insert(const_iterator position, size_type n, const_reference x);
    template <class InputIterator>
    void insert(const_iterator position, InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last);
	void move_range(pointer __from_s, pointer __from_e, pointer __to);
	template <class InputIterator, class OutputIterator>
	OutputIterator move(InputIterator first, InputIterator last, OutputIterator result);
	template <class InputIterator, class OutputIterator>
	OutputIterator move_backward(InputIterator first, InputIterator last, OutputIterator result);
	iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
	//Allocator:
	//Non-member function overloads
private :
	//utils
	void vallocate(size_type n);
	void vdeallocate();
	void construct_at_end(size_type n);
	void construct_at_end(size_type n, const_reference val);
	template <class InputIterator>
	void construct_at_end(
		InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type second, size_type n);
	size_type recommend(size_type new_size) const;
	void append(size_type n);
    void append(size_type n, const_reference x);
	struct ConstructTransaction 
	{
    explicit ConstructTransaction(vector &ref, size_type n)
      : CTV(ref),  pos(ref.__end_), new_end(ref.__end_ + n) {}
    ~ConstructTransaction() {
      CTV.__end_ = pos;
    }
    vector &CTV;
    pointer pos;
    const_pointer const new_end;
	};
};

//utils
template <class T, class Allocator>
typename vector<T, Allocator>::size_type 
vector<T, Allocator>::max_size() const
{
	return (std::min<size_type>(this->__alloc().max_size(), 
								std::numeric_limits<difference_type>::max()));
}
template <class T, class Allocator>
void vector<T, Allocator>::vallocate(size_type n)
{
	if (n > max_size())
		this->throwLengthError();
	this->__begin_ = this->__end_ = this->__alloc().allocate(n);
	this->__end_cap() = this->__begin_ + n;
}
template <class T, class Allocator>
void vector<T, Allocator>::vdeallocate()
{
	if (this->__begin_ != nullptr)
	{
		this->clear();
		this->__alloc().deallocate(this->__begin_, capacity());
		this->__begin_ = this->__end_ = this->__end_cap() = nullptr;
	}
}
template <class T, class Allocator>
void vector<T, Allocator>::construct_at_end(size_type n)
{
	ConstructTransaction tx(*this, n);
	for (; tx.pos != tx.new_end; ++tx.pos)
	{
		this->__alloc().construct(tx.pos);
	}
}
template <class T, class Allocator>
void vector<T, Allocator>::construct_at_end(size_type n, const_reference val)
{
	ConstructTransaction tx(*this, n);
	for (; tx.pos != tx.new_end; ++tx.pos)
	{
		this->__alloc().construct(tx.pos, val);
	}
}
template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::construct_at_end( InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last,
		size_type n)
{
	ConstructTransaction tx(*this, n);
	for (;first != last; ++first ,++tx.pos)
	{
		this->__alloc().construct(tx.pos, *first);
	}
}
template <class T, class Allocator>
typename vector<T, Allocator>::size_type 
vector<T, Allocator>::recommend(size_type new_size) const
{
	const size_type ms = max_size();
	if (new_size > ms)
		this->throwLengthError();
	const size_type capa = capacity();
	if (capa >= ms / 2)
		return (ms);
	return std::max<size_type>(2 * capa, new_size);
}
template <class T, class Allocator>
void vector<T, Allocator>::append(size_type n)
{
	if (static_cast<size_type>(this->__end_cap() - this->__end_) >= n)
		this->construct_at_end(n);
	else
	{
		allocator_type& a = this->__alloc();
		vector<T, Allocator> temp_vector(recommend(size() + n), 0, a);
		temp_vector.__end_ = std::copy(this->__begin_, this->__end_, temp_vector.__begin_);
		temp_vector.construct_at_end(n);
		std::swap(this->__begin_, temp_vector.__begin_);
		std::swap(this->__end_, temp_vector.__end_);
		std::swap(this->__end_cap(), temp_vector.__end_cap());
	}
}
template <class T, class Allocator>
void vector<T, Allocator>::append(size_type n, const_reference x)
{
	if (static_cast<size_type>(this->__end_cap() - this->__end_) >= n)
		this->construct_at_end(n, x);
	else
	{
		allocator_type& a = this->__alloc();
		vector<T, Allocator> temp_vector(recommend(size() + n), x, a);
		temp_vector.__end_ = std::copy(this->__begin_, this->__end_, temp_vector.__begin_);
		temp_vector.construct_at_end(n, x);
		std::swap(this->__begin_, temp_vector.__begin_);
		std::swap(this->__end_, temp_vector.__end_);
		std::swap(this->__end_cap(), temp_vector.__end_cap());
	}
}

//Member functions
template <class T, class Allocator>
vector<T, Allocator>::vector(const allocator_type& a) : base(a) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& x, const allocator_type& a) : base(a)
{
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(n,x);
	}
}
template <class T, class Allocator>
vector<T, Allocator>::vector(const vector& copy) : base(copy.__alloc())
{//값 복사 해야하는거 아닌가? 그냥 대입연산자 호출할까..
	size_type n = copy.size();
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(copy.__begin_, copy.__end_, n);
	}
}

template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(
		InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last, 
		const allocator_type& a) : base(a)
{
	size_type n = static_cast<size_type>(std::distance(first, last));
//	size_type n = static_cast<size_type>(last - first);
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(first, last, n);
	}
}
/*
template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& a)
{
	size_type n = static_cast<size_type>(std::distance(first, last));
	if (n > 0)
	{
		vallocate(n);
		construct_at_end(first, last, n);
	}
}*/
template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& ref)
{
	if (this != &ref)
	{
		base::copy_assign_alloc(ref);
		assign(ref.__begin_, ref.__end_);
		this->__end_cap() = this->__end_;
	}
	return (*this);
}
//iterator
template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::make_iter(const_pointer p) const
{
	return const_iterator(p);
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::make_iter(pointer p)
{
	return iterator(p);
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::begin()
{
	return (make_iter(this->__begin_));
}
template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::begin()	const
{
	return (make_iter(this->__begin_));
}
template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::end()
{
	return (make_iter(this->__end_));
}
template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::end()	const
{
	return (make_iter(this->__end_));
}

//Modifiers:
template <class T, class Allocator>
void vector<T, Allocator>::assign(size_type n, const_reference u)
{
	if (n <= capacity())
	{
		size_type s = size();
		std::fill_n(this->__begin_, std::min(n,s), u);
		if (n > s)
			this->construct_at_end(n - s, u);
		else
			this->destruct_at_end(this->__begin_ + n);
	}
	else
	{
		vdeallocate();
		vallocate(recommend(static_cast<size_type>(n)));
		construct_at_end(n, u);
	}
}
template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::assign( InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value && 
		is_same<typename iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value, InputIterator>::type last)
{
	clear();
	for (; first != last; ++first)
		this->append(1, *first);
}
template <class T, class Allocator>
template <class ForwardIterator>
void vector<T, Allocator>::assign( ForwardIterator first, 
		typename ft::enable_if<!ft::is_integral<ForwardIterator>::value && 
		(is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::forward_iterator_tag>::value || 
		is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::bidirectional_iterator_tag>::value || 
		is_same<typename iterator_traits<ForwardIterator>::iterator_category, std::random_access_iterator_tag>::value) , ForwardIterator>::type last)
{
	size_type new_size = static_cast<size_type>(std::distance(first, last));
    if (new_size <= capacity())
    {
        ForwardIterator mid = last;
        bool growing = false;
        if (new_size > size())
        {
            growing = true;
            mid =  first;
            std::advance(mid, size());
        }
        pointer m = std::copy(first, mid, this->__begin_);
        if (growing)
            construct_at_end(mid, last, new_size - this->size());
        else
            this->destruct_at_end(m);
    }
    else
    {
        vdeallocate();
        vallocate(this->recommend(new_size));
        construct_at_end(first, last, new_size);
    }
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const_reference x)
{
	this->append(1, x);
	/*
	if (this->__end_ != this->__end_cap())
	{
		construct_at_end(1, x);
	}
	else
	{
		allocator_type& a = this->__alloc();
		ft::vector<T> temp_vector(recommend(size() + 1), x, a);
		std::copy(this->__begin_, this->__end_, temp_vector.__begin_);
		std::swap(this->__begin_, temp_vector.__begin_);
		std::swap(this->__end_, temp_vector.__end_);
		std::swap(this->__end_cap(), temp_vector.__end_cap());
	}*/
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back()
{
	this->destruct_at_end(this->__end_ - 1);
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator position, const_reference x)
{
	pointer p = this->__begin_ + (position - this->begin());
	if (this->__end_ != this->__end_cap())
	{
		if (p == this->__end_)
			this->construct_at_end(1, x);
		else
		{
			move_range(p, this->__end_, p + 1);
			const_pointer xr = &x;
			if (p <= xr && xr < this->__end_)
				++xr;
			*p = *xr;
		}
	}
	else
	{
		allocator_type& a = this->__alloc();
		vector<T, Allocator> temp_vector(recommend(size() + 1), 0, a);
		temp_vector.__end_ = std::copy(this->__begin_, p, temp_vector.__begin_);
		temp_vector.construct_at_end(1, x);
		temp_vector.__end_ = std::copy(p, this->__end_, temp_vector.__end_);
		p = temp_vector.__begin_ + (position - this->begin());
		std::swap(this->__begin_, temp_vector.__begin_);
		std::swap(this->__end_, temp_vector.__end_);
		std::swap(this->__end_cap(), temp_vector.__end_cap());
	}
	return (make_iter(p));
}

template <class T, class Allocator>
void vector<T, Allocator>::insert(const_iterator position, size_type n, const_reference x)
{
	pointer p = this->__begin_ + (position - this->begin());
	if (n > 0)
	{
		if (n <= static_cast<size_type>(this->__end_cap() - this->__end_))
		{
			size_type old_n = n;
			pointer old_last = this->__end_;
			if (n > static_cast<size_type>(this->__end_ - p))
			{
				size_type cx = n - (this->__end_ - p);
				construct_at_end(cx, x);
				n -= cx;
			}
			if (n > 0)
			{
				move_range(p, old_last, p + old_n);
				const_pointer xr = &x;
				if (p <= xr && xr < this->__end_)
					xr += old_n;
				std::fill_n(p, n, *xr);
			}
		}
		else
		{
			allocator_type& a = this->__alloc();
			vector<T, Allocator> temp_vector(recommend(size() + n), 0, a);
			temp_vector.__end_ = std::copy(this->__begin_, p, temp_vector.__begin_);
			temp_vector.construct_at_end(n, x);
			temp_vector.__end_ = std::copy(p, this->__end_, temp_vector.__end_);
			p = temp_vector.__begin_ + (position - this->begin());
			std::swap(this->__begin_, temp_vector.__begin_);
			std::swap(this->__end_, temp_vector.__end_);
			std::swap(this->__end_cap(), temp_vector.__end_cap());
		}
	}
}
template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::insert(const_iterator position, InputIterator first,
		typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last)
{
	size_type pos_dis = position - this->begin();
    difference_type n = std::distance(first, last);
//	std::cout << size()+n << std::endl;
	reserve(recommend(size() + n));
	pointer old_end = this->__end_;
	pointer p = this->__begin_ + pos_dis;
	try
	{
		construct_at_end(first,last, n);
	}
	catch(...)
	{
		this->destruct_at_end(old_end);
		vector<T, Allocator> temp;
		temp.vallocate(this->size());
		temp.construct_at_end(this->__begin_, this->__end_, n);
		swap(temp);
		throw;
	}
	if (p != old_end)
		std::rotate(make_iter(p), make_iter(old_end), make_iter(this->__end_));
}

template <class T, class Allocator>
template <class InputIterator, class OutputIterator>
OutputIterator vector<T, Allocator>::move_backward
(InputIterator first, InputIterator last, OutputIterator result)
{
	while (first != last)
		*--result = *--last;
	return (result);
}

template <class T, class Allocator>
void vector<T, Allocator>::move_range(pointer from_s, pointer from_e, pointer to)
{
	pointer old_last = this->__end_;
	difference_type n = old_last - to;
	pointer i = from_s + n;
	ConstructTransaction tx(*this, from_e - i);
	for (pointer pos = tx.pos; i < from_e; ++i, ++pos, tx.pos = pos)
	{
		this->__alloc().construct(pos, *i);
	}
	move_backward(from_s, from_s + n , old_last);
}

template <class T, class Allocator>
template <class InputIterator, class OutputIterator>
OutputIterator vector<T, Allocator>::move
(InputIterator first, InputIterator last, OutputIterator result)
{
	for (; first != last; ++first, ++result)
		*result = *first;
	return (result);
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator 
vector<T, Allocator>::erase(const_iterator position)
{
	difference_type ps = position - this->begin();
	pointer p = this->__begin_ + ps;
	this->destruct_at_end(this->move(p + 1, this->__end_, p));
//	iterator r = make_iter(p);
	return (p);
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator 
vector<T, Allocator>::erase(const_iterator first, const_iterator last)
{
	pointer p = this->__begin_ + (first - this->begin());
	if (first != last)
	{
		this->destruct_at_end(this->move(p + (last - first), this->__end_, p));
	}
//	iterator r = make_iter(p);
	return (p);
}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector& x)
{
	std::swap(this->__begin_, x.__begin_);
	std::swap(this->__end_, x.__end_);
	std::swap(this->__end_cap(), x.__end_cap());
	std::swap(this->__alloc(), x.__alloc());
}
//Capacity
template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type sz)
{
	size_type cs = size();
	if (cs < sz)
		this->append(sz - cs);
	else
		this->destruct_at_end(this->__begin_ + sz);
}
template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type sz, const_reference x)
{
	size_type cs = size();
	if (cs < sz)
		this->append(sz - cs, x);
	else
		this->destruct_at_end(this->__begin_ + sz);
}
template <class T, class Allocator>
void  vector<T, Allocator>::reserve(size_type n)
{
	const size_type ms = max_size();
	if (n > ms)
		this->throwLengthError();
	if (n > capacity())
	{
		allocator_type& a = this->__alloc();
		vector<T, Allocator> temp_vector(a);
		temp_vector.vallocate(n);
	//	temp_vector.__end_ = std::copy(this->__begin_, this->__end_, temp_vector.__begin_);
		temp_vector.construct_at_end(this->__begin_, this->__end_, n);
		std::swap(this->__begin_, temp_vector.__begin_);
		std::swap(this->__end_, temp_vector.__end_);
		std::swap(this->__end_cap(), temp_vector.__end_cap());
	}
}

//Element access:
template <class T, class Allocator>
typename vector<T, Allocator>::reference 
vector<T, Allocator>::operator[](size_type n)
{
	return (this->__begin_[n]);
}
template <class T, class Allocator>
typename vector<T, Allocator>::const_reference 
vector<T, Allocator>::operator[](size_type n) const
{
	return (this->__begin_[n]);
}
template <class T, class Allocator>
typename vector<T, Allocator>::reference 
vector<T, Allocator>::at(size_type n)
{
	if (n >= size())
		this->throwOutOfRange();
	return (this->__begin_[n]);
}
template <class T, class Allocator>
typename vector<T, Allocator>::const_reference 
vector<T, Allocator>::at(size_type n) const
{
	if (n >= size())
		this->throwOutOfRange();
	return (this->__begin_[n]);
}
//non_member
template <class T, class Allocator>
bool operator==(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    const typename vector<T, Allocator>::size_type sz = x.size();
    return sz == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
bool operator!=(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    return !(x == y);
}

template <class T, class Allocator>
bool operator< (const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Allocator>
bool operator> (const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    return y < x;
}

template <class T, class Allocator>
bool operator>=(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    return !(x < y);
}

template <class T, class Allocator>
bool operator<=(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
{
    return !(y < x);
}
template< class T, class Alloc >
void swap(ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs)
{
	lhs.swap(rhs);
}
};
#endif