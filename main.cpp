
#include "Vector.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
int main () 
{
//	std::vector<int> tmep4(1);
//	ft::Vector<int> temp1;
	ft::Vector<int> temp2(4,100);
	ft::Vector<int> temp1(4,200);
//	ft::Vector<int> temp3(temp2.begin(), temp2.end());
	ft::swap(temp1, temp2);
	std::cout << temp2.back() << std::endl;
/*	std::cout << temp2[0] << std::endl;
	std::cout << temp2.at(0) << std::endl;
	std::cout << *temp2.begin() << std::endl;
	std::cout << temp2.capacity() << std::endl;
	std::cout << temp2.back() << std::endl;
	std::cout << temp2.front() << std::endl;
	std::cout << temp2.empty() << std::endl;
	temp2.reserve(10);
	std::cout << temp2.capacity() << std::endl;
	temp2.resize(20);
	std::cout << temp2.capacity() << std::endl;
	std::cout << *temp2.end() << std::endl;
	std::cout << temp2.size() << std::endl;
	std::cout <<temp2.max_size() << std::endl;*/
	std::cout << *temp2.end() << std::endl;
	temp2.assign(1, 2);
	std::cout << temp2.size() << std::endl;
	std::cout << temp2.back() << std::endl;
//	std::cout << *temp2.end() << std::endl;
//	std::cout << temp2[0] << std::endl;
	temp2.push_back(14);
	std::cout << temp2.back() << std::endl;
//	std::cout << temp2[0] << std::endl;
//	std::cout << *temp2.end() << std::endl;
	temp2.pop_back();
	std::cout << temp2.back() << std::endl;
	temp2.clear();
	std::cout << temp2.size() << std::endl;
//	std::cout << temp2[0] << std::endl;
//	std::cout << temp2.size() << std::endl;
}
/*
insert	Insert elements (public member function)
erase	Erase elements (public member function)
swap	Swap content (public member function)
clear*/