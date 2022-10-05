#include "Pair.hpp"
#include "Vector.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
int main () 
{
	ft::Vector<int> temp1;
	ft::Vector<int> temp2(4,100);
	ft::Vector<int> temp3(temp2.begin(), temp2.end());


	std::cout << temp2[0] << std::endl;
	std::cout << *temp2.begin() << std::endl;
}
