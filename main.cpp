#include "vector.hpp"
#include "common.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/time.h>
volatile static time_t g_start1;
volatile static time_t g_start2;
volatile static time_t g_end1;
volatile static time_t g_end2;
const std::string GREEN = "\x1B[1;32m";
const std::string REDD = "\x1B[1;31m";
const std::string YELLOW = "\x1B[1;33m";
const std::string WHITE = "\x1B[1;39m";
const std::string RESET = "\033[0m";
int _test_passed_cnt = 0;
int _ratio = 10000;
void printElement(std::string t) {
	if (t == "OK")
	    t = GREEN + t + RESET;
	else if (t == "FAILED") t = REDD + t + RESET;
	std::cout << std::left << std::setw(30) << std::setfill(' ') << t;
}
time_t timer() {
	struct timeval start = {};
	gettimeofday(&start, nullptr);
	time_t msecs_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	return msecs_time;
}

template <class T>
int run_stack_unit_test(std::string test_name, std::vector<int> (func1)(std::stack<T>), std::vector<int> (func2)(ft::stack<T>)) {
    int    result;

    time_t t1;
	time_t t2;
	std::vector<int > res1;
	std::vector<int > res2;
	std::stack<int> stack;
	ft::stack<int> my_stack;

	printElement(test_name);
	res1 = func1(stack);
	res2 = func2(my_stack);
	if (res1 == res2) {
	    printElement("OK");
	    result = 0;
	}
	else {
	    printElement("FAILED");
	    result = 1;
	}
	t1 = g_end1 - g_start1, t2 = g_end2 - g_start2;
	(t1 >= t2) ? printElement(GREEN + std::to_string(t2) + "ms" + RESET) : printElement(REDD + std::to_string(t2) + "ms" + RESET);
	(t1 > t2) ? printElement(REDD + std::to_string(t1) + "ms" + RESET) : printElement(GREEN + std::to_string(t1) + "ms" + RESET);

	std::cout << std::endl;

	return !(!result );;
}
template <class T>
std::vector<int> empty_test(std::stack<T> stk) {
	std::vector<int> v;
	for (int i = 0; i < 200 * _ratio; ++i)
		stk.push(i);
	v.push_back(stk.empty());
	while (stk.size() > 0)
		stk.pop();
	g_start1 = timer();
	v.push_back(stk.empty());
	g_end1 = timer();
	return v;
}

template <class T>
std::vector<int> empty_test(ft::stack<T> stk) {
	std::vector<int> v;
	for (int i = 0; i < 200 * _ratio; ++i)
		stk.push(i);
	v.push_back(stk.empty());
	while (stk.size() > 0)
		stk.pop();
	g_start2 = timer();
	v.push_back(stk.empty());
	g_end2 = timer();
	return v;
}

int main() {

	exit(run_stack_unit_test<int>("empty()", empty_test, empty_test));
}