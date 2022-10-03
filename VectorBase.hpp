#ifndef VECTORBASE_HPP
#define	VECTORBASE_HPP
#include "VectorBaseCommon.hpp"
#include "Pair.hpp"
#include <memory>
#include <utility>
namespace ft
{
template <class T, class Allocator>
class VectorBase : protected VectorBaseCommon
{
public:
    typedef Allocator                                allocator_type;
    typedef std::allocator_traits<allocator_type>    alloc_traits;
    typedef typename alloc_traits::size_type       	 size_type;
protected:
    typedef T	                                     value_type;
    typedef value_type&                              reference;
    typedef const value_type&                        const_reference;
    typedef typename alloc_traits::difference_type	 difference_type;
    typedef typename alloc_traits::pointer           pointer;
    typedef typename alloc_traits::const_pointer     const_pointer;
    typedef pointer                                  iterator;
    typedef const_pointer                            const_iterator;

    pointer                                         begin;
    pointer                                         end;
	ft::pair<pointer, allocator_type> end_cap_val;
	allocator_type& alloc() {return end_cap_val.second();}
    const allocator_type& alloc() {return end_cap_val.second();}
    pointer& end_cap() {return end_cap_val.first();}
    const pointer& end_cap() {return end_cap_val.first();}

	VectorBase();
	VectorBase(const allocator_type& a);
	~VectorBase();
	void clear() { destruct_at_end(begin); }
	size_type capacity()
		{ return static_cast<size_type>(end_cap() - begin); }
	void destruct_at_end(pointer new_last);
};

template <class T, class Allocator>
void	VectorBase<T, Allocator>::destruct_at_end(pointer new_last)
{
	pointer soon_to_be_end = end;
	while (new_last != soon_to_be_end)
		alloc_traits::destroy(alloc(), --soon_to_be_end);
	end = new_last;
}

template <class T, class Allocator>
VectorBase<T, Allocator>::VectorBase() 
	:	begin(nullptr), 
		end(nullptr), 
		end_cap_val(nullptr, std::__default_init_tag){}

template <class T, class Allocator>
VectorBase<T, Allocator>::VectorBase(const allocator_type& a)
	:	begin(nullptr), 
		end(nullptr), 
		end_cap_val(nullptr, a){}

template <class T, class Allocator>
VectorBase<T, Allocator>::~VectorBase()
{
	if (begin != nullptr)
	{
		clear();
		alloc_traits::deallocate(alloc(), begin, capacity());
	}
}

}
#endif