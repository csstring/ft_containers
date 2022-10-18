#include <iostream>
#include "utility.hpp"
#include <string>
#include <list>
#define T1 int
#define T2 std::string
typedef ft::pair<const T1, T2> T3;



struct _Rb_tree_node_base
{
	typedef _Rb_tree_node_base* _Base_ptr;
	typedef const _Rb_tree_node_base* _Const_Base_ptr;
	_Base_ptr		_M_parent;
	_Base_ptr		_M_left;
	_Base_ptr		_M_right;
	bool	_M_color;
};

struct test
{int* temp2;
	int temp;
//	char	_M_color;
};

template<typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base
{	
	_Val _M_value_field;
};

int main(void)
{
	std::pair <int*, char> product1;
	std::list<T3> lst;
	unsigned int lst_size = 10;
	for (unsigned int i = 0; i < lst_size; ++i)
		lst.push_back(T3(i, std::string((lst_size - i), i + 65)));
	std::cout << "list iter size: " << sizeof(lst.begin()) << std::endl;
	ft::pair <std::list<T3>::iterator , std::list<T3>::iterator> product2(lst.begin(), lst.end());
	std::cout << "product1 size: " << sizeof(product1) << std::endl;
	std::cout << "product2 size: " << sizeof(product2) << std::endl;

	test temp;
	std::cout << "temp size: " << sizeof(temp) << std::endl;
	return 0;
}