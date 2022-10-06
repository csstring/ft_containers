#ifndef VECTORBASE_HPP
#define	VECTORBASE_HPP
#include "VectorBaseCommon.hpp"
#include "utility.hpp"
#include <memory>
#include <utility>
#include <iostream>
namespace ft
{
template <class T, class Allocator = std::allocator<T> >
class VectorBase : protected VectorBaseCommon
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

    pointer                                         begin_;
    pointer                                         end_;
	ft::pair<pointer, allocator_type> end_cap_val;
	allocator_type& alloc() {return end_cap_val.second;}
    const allocator_type& alloc() const {return end_cap_val.second;}
    pointer& end_cap() {return end_cap_val.first;}
    const pointer& end_cap() const {return end_cap_val.first;}

	VectorBase();
	VectorBase(const allocator_type& a);
	~VectorBase();
	void clear() { destruct_at_end(begin_); }
	size_type capacity() const
		{ return static_cast<size_type>(end_cap() - begin_); }
	void destruct_at_end(pointer new_last);
private:
	void copy_assign_alloc(const VectorBase& c)
	{
		if (alloc() != c.alloc())
		{
			clear();
			alloc().deallocate(begin_, capacity());
			begin_ = end_ = end_cap() = nullptr;
		}
		alloc() = c.alloc();
	}
};

template <class T, class Allocator>
void	VectorBase<T, Allocator>::destruct_at_end(pointer new_last)
{
	pointer soon_to_be_end = end_;
	while (new_last != soon_to_be_end)
		alloc().destroy(--soon_to_be_end);
	end_ = new_last;
}

template <class T, class Allocator>
VectorBase<T, Allocator>::VectorBase() 
	:	begin_(nullptr), 
		end_(nullptr), 
		end_cap_val(nullptr, std::__default_init_tag()){}

template <class T, class Allocator>
VectorBase<T, Allocator>::VectorBase(const allocator_type& a)
	:	begin_(nullptr), 
		end_(nullptr), 
		end_cap_val(nullptr, a){}

template <class T, class Allocator>
VectorBase<T, Allocator>::~VectorBase()
{
	if (begin_ != nullptr)
	{
		//Allocator a = alloc();
		clear();
		alloc().deallocate(begin_, capacity());
	}
}

}
#endif