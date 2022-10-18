#include "set.hpp"
#include "vector.hpp"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/time.h>
#define	_vector	 			ft::vector
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

template <class T>
int run_vector_unit_test(std::string test_name, std::vector<int> (func1)(std::vector<T>), std::vector<int> (func2)(_vector<T>)) {
    int    result;
    time_t t1;
    time_t t2;
    std::vector<int > res1;
    std::vector<int > res2;
    std::vector<int> vector;
    _vector<int> my_vector;

	printElement(test_name);
	res1 = func1(vector);
	res2 = func2(my_vector);
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

template <typename T>
std::vector<int> swap_test(std::vector<T> vector) {
    std::vector<int> v;
    vector.assign(1100 * _ratio, 11);
    std::vector<int> tmp(500 * _ratio, 5), tmp2(1000 * _ratio, 10), tmp3(1500 * _ratio, 15), tmp4(3000 * _ratio, 30);
    g_start1 = timer();
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    long *adr1 = reinterpret_cast<long *>(&vector);
    long *adr2 = reinterpret_cast<long *>(&tmp);
    vector.swap(tmp);
    if (reinterpret_cast<long *>(&vector) == adr1 && reinterpret_cast<long *>(&tmp) == adr2)
    	v.push_back(1);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    vector.swap(tmp3);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    std::swap(vector, tmp2);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    std::swap(vector, tmp4);
    g_end1 = timer();
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    return v;
}

template <typename T>
std::vector<int> swap_test(_vector<T> vector) {
    std::vector<int> v;
    vector.assign(1100 * _ratio, 11);
    _vector<int> tmp(500 * _ratio, 5), tmp2(1000 * _ratio, 10), tmp3(1500 * _ratio, 15), tmp4(3000 * _ratio, 30);
    g_start2 = timer();
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    long *adr1 = reinterpret_cast<long *>(&vector);
    long *adr2 = reinterpret_cast<long *>(&tmp);
    vector.swap(tmp);
    if (reinterpret_cast<long *>(&vector) == adr1 && reinterpret_cast<long *>(&tmp) == adr2)
    	v.push_back(1);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    vector.swap(tmp3);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    ft::swap(vector, tmp2);
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    ft::swap(vector, tmp4);
    g_end2 = timer();
    v.push_back(vector[2]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());
    return v;
}

int main() {

    exit(run_vector_unit_test<int>("swap()", swap_test, swap_test));
}