#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "nlohmann/json.hpp"

#define A_FILE   "A_struct.json"
#define B_FILE   "B_struct.json"

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;

json merge(const json &a, const json &b)
{
    json result = a.flatten();
    json tmp = b.flatten();

    for (json::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        result[it.key()] = it.value();
    }

    return result.unflatten();
}

class A {
public:
	int32_t k;
    int a[10];
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(A, k, a);

	A(int window) {
		this->k = window;
	}
    A() {
		this->k = 987;
	}

    void printA() {
        std::cerr << "k:\t" << k << std::endl;
        std::cerr << "a:";

        for (int i = 0; i < 10; i++) {
            std::cerr << '\t' << a[i] << std::endl;
        }
        cerr << endl;
    }

    void Save() {
        std::ofstream out(A_FILE);
        json j;

        to_json(j, *this);

        out << j.dump(4) << std::endl;
        out.close();
    }

    void Load() {
        std::ifstream inp(A_FILE);
        if (!inp.is_open()) return;
        json ji;

        inp >> ji;
        from_json(ji, *this);
        return;    
    }

};
    
class B {
public:
    int x;
    int y;
    A* ptr;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(B, x, y); 
    //NLOHMANN_DEFINE_TYPE_INTRUSIVE(B, x, y, ptr); - error

    B() {
        this->x = 0;
        this->y = 1;
        ptr = nullptr;
    }

    B(A* base) : x(base->k), y(base->k), ptr(base) {}

    void printB() {
        cerr << "x:\t" << x << endl
             << "y:\t" << y << endl
             << "*ptr:\t";
        if (ptr != nullptr) ptr->printA();
        cerr << endl;
    }

    void Save() {
        std::ofstream out(B_FILE);
        json j;

        to_json(j, *this);

        out << j.dump(4) << std::endl;
        out.close();
    }

    void Load() {
        std::ifstream inp(B_FILE);
        if (!inp.is_open()) return;
        json ji;

        inp >> ji;
        from_json(ji, *this);
        return;    
    }
};


template<class T>
void serealize_json(T* a, std::string fname) {
    std::ofstream out(fname);
    json j;

    to_json(j, *a);

    out << j.dump(4) << std::endl;
    out.close();
}

template<class T>
void deserealize_json(T* b, std::string fname) {
    std::ifstream inp(fname);
    json ji;
    
    inp >> ji;
    from_json(ji, *b);
    return;
}
