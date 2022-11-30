#include "header.hpp"

int main(int argc, char* argv[]) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //загрузка/сохранение состояний
    A aaa(25); aaa.Save();
    A aParse; aParse.Load();
    B bbb{&aParse}; bbb.Save();
    B bParse; bParse.Load();
    // aParse.printA("aParse");
    // bParse.printB("bParse");
    // aaa.printA("aaa");
    // bbb.printB("bbb");

    //сохранение B (вместе с A как поле (через указатель))
    json jA; to_json(jA, aaa);
    json jB; to_json(jB, bbb);
    json jBsub; to_json(jBsub, bbb);
    json jBmerge; to_json(jBmerge, bbb);

    jBmerge = merge(jA, jBmerge);
    jBsub = mergeSub(jBsub, jA);
    
    ofstream outs("test_Sub.json");
    ofstream outm("test_Merge.json");
    ofstream outA("test_A.json");
    ofstream outB("test_B.json");
    
    outs << jBsub.dump(4) << std::endl; outs.close();
    outm << jBmerge.dump(4) << std::endl; outm.close();
    outA << jA.dump(4) << std::endl; outA.close();
    outB << jB.dump(4) << std::endl; outB.close();

    //загрузка B вместе с A
    ofstream outSub(SUB_FILE);
    outSub << jBsub.dump(4) << std::endl; outSub.close();

    B bSPars; bSPars.Load_full();
    bSPars.printB("B Parsed with A");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << endl;
    std::cout << "Time of working: "<< duration << endl;
}
