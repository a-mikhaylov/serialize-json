#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "nlohmann/json.hpp"

#define A_FILE   "A_struct.json"
#define B_FILE   "B_struct.json"
#define SUB_FILE "SUB_AB.json"

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

json mergeSub(const json &base, const json &dist) {
    json to_add;
    to_add = to_add.flatten();
    json result = base.flatten();
    json tmp = dist.flatten();
    
    for (json::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        to_add[it.key()] = it.value();
    }
    
    result = result.unflatten();
    to_add = to_add.unflatten();
    result["Added"] = to_add;
    
    return result;
}

class A {
public:
	int32_t k;
    int a[10];
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(A, k, a);

	A(int window) {
		this->k = window;
        for (int i = 0; i < 10; i++) a[i] = rand() % 10;
	}
    A() {
		this->k = 987;
	}

    void printA(std::string name) {
        cerr << name << ":" << endl;
        cerr << "\tk:\t" << k << endl;
        cerr << "\ta:" << endl;;

        for (int i = 0; i < 10; i++) {
            cerr << "\t\t" << a[i] << endl;
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

    void Load(const json &base) { 
        from_json(base, *this); 
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
        ptr = new A();
    }

    B(A* base) : x(base->k), y(base->k), ptr(base) {}

    void printB(std::string name) {
        cerr << name << ":" << endl;
        cerr << "\tx:\t" << x << endl
             << "\ty:\t" << y << endl
             << "\t*ptr:";
        if (ptr != nullptr) ptr->printA("*ptr");
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

    void Load_full() {
        std::ifstream inp(SUB_FILE);
        if (!inp.is_open()) return;
        json ji;
        inp >> ji;

        json jA = ji["Added"];
        ptr->Load(jA);

        ji.erase("Added");
        from_json(ji, *this);
        return;
    }
    ~B() { 
        if (ptr != nullptr) {
            delete ptr;
            ptr = nullptr;
        }
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
