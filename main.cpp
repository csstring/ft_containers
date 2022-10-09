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
int run_vector_unit_test(std::string test_name, std::vector<int> (func1)(std::vector<T>), std::vector<int> (func2)(ft::vector<T>)) {
    int    result;
    time_t t1;
    time_t t2;
    std::vector<int > res1;
    std::vector<int > res2;
    std::vector<int> vector;
    ft::vector<int> my_vector;

	printElement(test_name);
	res1 = func1(vector);
	res2 = func2(my_vector);
	std::cout << std::endl;
	for (unsigned long i = 0; i < res1.size(); i++)
		std::cout << res1[i] << std::endl;
	for (unsigned long i = 0; i < res2.size(); i++)
		std::cout << res2[i] << std::endl;	
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

	return !(!result );
}
class B {
public:
    char *l;
    int i;
    B():l(nullptr), i(1) {};
    B(const int &ex) {
        this->i = ex;
        this->l = new char('a');
    };
    virtual ~B() {
        delete this->l;
        this->l = nullptr;
    };
};

class A : public B {
public:
    A():B(){};
    A(const B* ex){
        this->l = new char(*(ex->l));
        this->i = ex->i;
        if (ex->i == -1) throw "n";
    }
    ~A() {
        delete this->l;
        this->l = nullptr;
    };
};

template <typename T>
std::vector<int> insert_test_3(std::vector<T> vector) {
    std::vector<int> v;
    std::vector<int> tmp;
    tmp.assign(2600 * _ratio, 1);
    vector.assign(4200 * _ratio, 1);
    g_start1 = timer();
    vector.insert(vector.end() - 1000 * _ratio, tmp.begin(), tmp.end());
    g_end1 = timer();
    v.push_back(vector[3]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());

    std::unique_ptr<B> k2(new B(3));
    std::unique_ptr<B> k3(new B(4));
    std::unique_ptr<B> k4(new B(-1));
    std::vector<A> vv;
    std::vector<B*> v1;

    v1.push_back(&(*k2));
    v1.push_back(&(*k3));
    v1.push_back(&(*k4));
    try { vv.insert(vv.begin(), v1.begin(), v1.end()); }
    catch (...) {std::cout << "origin catch check"<< std::endl;
		std::cout << vv.size()<< std::endl;
		std::cout << vv.capacity()<< std::endl;
        v.push_back(vv.size());
        v.push_back(vv.capacity());
    }

    return v;
}

template <typename T>
std::vector<int> insert_test_3(ft::vector<T> vector) {
    std::vector<int> v;
    ft::vector<int> tmp;
    tmp.assign(2600 * _ratio, 1);
    vector.assign(4200 * _ratio, 1);
    g_start2 = timer();
    vector.insert(vector.end() - 1000 * _ratio, tmp.begin(), tmp.end());
    g_end2 = timer();
    v.push_back(vector[3]);
    v.push_back(vector.size());
    v.push_back(vector.capacity());

    std::unique_ptr<B> k2(new B(3));
    std::unique_ptr<B> k3(new B(4));
    std::unique_ptr<B> k4(new B(-1));
    ft::vector<A> vv;
    ft::vector<B*> v1;

    v1.push_back(&(*k2));
    v1.push_back(&(*k3));
    v1.push_back(&(*k4));
    try { vv.insert(vv.begin(), v1.begin(), v1.end()); }
    catch (...) {
		std::cout << "catch check"<< std::endl;
        v.push_back(vv.size());
        v.push_back(vv.capacity());
		std::cout << vv.size()<< std::endl;
		std::cout << vv.capacity()<< std::endl;
    }

    return v;
}

int main() {

    exit(run_vector_unit_test<int>("insert(range)", insert_test_3, insert_test_3));
}