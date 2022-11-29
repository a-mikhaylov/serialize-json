#include "header.hpp"

int main(int argc, char* argv[]) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    A aaa(25); aaa.printA(); aaa.Save();
    
    A aParse; aParse.Load(); aParse.printA();

    B bbb{&aParse}; bbb.printB(); bbb.Save();

    B bParse; bParse.Load(); bParse.printB();


    json jA; to_json(jA, aaa);
    json jB; to_json(jB, bbb);

    jB = merge(jA, jB);
    ofstream out("test_insert.json");
    out << jB.dump(4) << std::endl;
    out.close();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << endl << endl;
    std::cout << "Time of working: "<< duration << endl;
}
