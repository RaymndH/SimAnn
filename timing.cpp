#include <stdint.h>
#include <iostream>
#include <chrono>
#include <random>
int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());

    int s = 1e5;
    std::vector<uint32_t> intvec(s, 0);
    //typedef uint_fast8_t mytype;
    typedef uint8_t mytype;
    std::vector<mytype> charvec(s, 0);
    std::vector<std::vector<uint8_t>> vecvec(s/2, std::vector<uint8_t>(2,1));
    for(auto& el : intvec)
        el = gen() % 5;
    for(auto& el : charvec)
        el = gen() % 5;
    for(auto& el : vecvec)
        for(auto& el1 : el)
            el1 = gen() % 5;

    int counter = 0;


    auto start = std::chrono::high_resolution_clock::now();

    for(auto& el : charvec){
        //++el;
        if (el < 3)
            {++counter;}}

    std::chrono::duration<double> elapsedc = std::chrono::high_resolution_clock::now() - start;

    std::cout << "char " << elapsedc.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    /////////////////////
    counter = 0;
    start = std::chrono::high_resolution_clock::now();

    for(auto& el : intvec){
        if (el < 3)
            {++counter;}}

    std::chrono::duration<double> elapsedi = std::chrono::high_resolution_clock::now() - start;
    std::cout << "int " << elapsedi.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    ///////////
    counter = 0;
    start = std::chrono::high_resolution_clock::now();

    for(auto& el : intvec)
        counter+=(el >= 3);

    std::chrono::duration<double> elapsedicond = std::chrono::high_resolution_clock::now() - start;
    std::cout << "int " << elapsedicond.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    /////////////
    counter = 0;
    start = std::chrono::high_resolution_clock::now();

    for(auto& el : vecvec)
        for(auto& el1 : el) {
            if (el1 < 3){ 
                ++counter;
            }
        }

    std::chrono::duration<double> elapsedv = std::chrono::high_resolution_clock::now() - start;


    std::cout << "vecvec " << elapsedv.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    std::cout << "ratio " << elapsedi/elapsedicond << "\n";

    //std::cout << "\n" << unsigned(charvec[100]) << "\n";
}