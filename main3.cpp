#include <iostream>
enum _Rb_tree_color { _S_red = false, _S_black = true };
int main(void)
{
	_Rb_tree_color	_M_color;
	bool __is_black_;

	std::cout << "enum size: " << sizeof(_M_color) << std::endl;
	std::cout << "bool size : " << sizeof(__is_black_) << std::endl;
	return 0;
}