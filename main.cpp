#include "set.hpp"
#include <set>
#include <vector>
#include <iostream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/time.h>
#define	_set	 			ft::set
#define	_is_integral 		ft::is_integral
#define	_enable_if 			ft::enable_if
#define	_pair 				ft::pair
#define	_make_pair 			ft::make_pair
#define	_lexicographical	ft::lexicographical_compare
#define	_equal				ft::equal
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

int run_bool_unit_test(std::string test_name, bool (func1)()) {
    int ret = 0;
    time_t t1;
    time_t t2;
    bool res;

	printElement(test_name);
	res = func1();
	if (res) {
	    printElement("OK");
	    ret = 0;
	}
	else {
	    printElement("FAILED");
	    ret = 1;
	}
	t1 = g_end1 - g_start1, t2 = g_end2 - g_start2;
	(t1 >= t2) ? printElement(GREEN + std::to_string(t2) + "ms" + RESET) : printElement(REDD + std::to_string(t2) + "ms" + RESET);
	(t1 > t2) ? printElement(REDD + std::to_string(t1) + "ms" + RESET) : printElement(GREEN + std::to_string(t1) + "ms" + RESET);
	std::cout << std::endl;

    return ret;
}

template <class T>
int run_set_unit_test(std::string test_name, std::vector<int> (func1)(std::set<T>), std::vector<int> (func2)(_set<T>)) {
    int    result;
    time_t t1;
	time_t t2;
	std::vector<int > res1;
	std::vector<int > res2;
	std::set<int> set;
	_set<int> my_set;

	printElement(test_name);
	res1 = func1(set);
	res2 = func2(my_set);
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

	return !(!result);
}

template <class T>
std::vector<int> copy_constructor_test(std::set<T> st) {
    std::vector<int> v;

    for (int i = 0; i < 30 * _ratio; ++i) {
        st.insert(i);
    }
    g_start1 = timer();
    std::set<int> st2(st.begin(), st.end());
    g_end1 = timer();
    std::set<int>::iterator it = st2.begin();
    for (int i = 0; i < 30 * _ratio; ++i, it++) {
        v.push_back(*it);
    }
    return v;
}

template <class T>
std::vector<int> copy_constructor_test(_set<T> st) {
    std::vector<int> v;

    for (int i = 0; i < 30 * _ratio; ++i)
        st.insert(i);
    g_start2 = timer();
    _set<int> st2(st.begin(), st.end());
    g_end2 = timer();
    _set<int>::iterator it = st2.begin();
    for (int i = 0; i < 30 * _ratio; ++i, it++) {
        v.push_back(*it);
    }
    return v;
}

int main() {
    exit(run_set_unit_test<int>("constructor", copy_constructor_test, copy_constructor_test));
}