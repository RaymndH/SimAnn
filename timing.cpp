#include <stdint.h>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

template<typename T>
bool decider(T de, float temp) {
    //std::cout << std::exp(-float((de-2))/temp) << " ";
    //std::cout << unsigned(de) << " ";
    if( std::exp(-(float(de)-2)/temp) < .5) {
        return(1);
    }
    return(0);
}
template<typename T>
bool decider2(T de, float temp) {
    return( (float(de)-2) / temp < std::log(.5) );
}

int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());

    int s = 1e5;
    struct site {
        int color;
    };
    std::vector<uint32_t> intvec(s, 0);
    typedef uint_fast8_t mytype;
    float temp = 1.9;
    //typedef uint8_t mytype;
    std::vector<mytype> charvec(s, 0);
    std::vector<std::vector<mytype>> vecvec(s/2, std::vector<mytype>(2,1));
    std::vector<site> structvec(s);
    for(auto& el : intvec)
        el = gen() % 5;
    for(auto& el : charvec)
        el = gen() % 5;
    for(auto& el : vecvec)
        for(auto& el1 : el)
            el1 = gen() % 5;
    for(auto& el : structvec)
        el.color = gen() % 5;
      

    
    int counter = 0;
    int numreps = 1e4;

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        for(auto& el : charvec) {
            //++el;
            counter += decider(el,temp);
                }
    }

    std::chrono::duration<double> elapsedc = std::chrono::high_resolution_clock::now() - start;

    std::cout << "char " << elapsedc.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    /////////////////////
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        for(auto el : intvec){
            if(decider(el,temp)) {counter ++;}}
    }
    std::chrono::duration<double> elapsedi = std::chrono::high_resolution_clock::now() - start;
    std::cout << "int " << elapsedi.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    ///////////
    counter = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        for(auto& el : intvec)
            counter += decider2(el,temp);
    }
    std::chrono::duration<double> elapsedicond = std::chrono::high_resolution_clock::now() - start;
    std::cout << "int " << elapsedicond.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    /////////////
    /*
    counter = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        for(auto& el : vecvec)
            for(auto& el1 : el) {
                if (el1 < 3){ 
                    ++counter;
                }
            }
    }

    std::chrono::duration<double> elapsedv = std::chrono::high_resolution_clock::now() - start;
    */
    ////////////// vector of structs of int
    counter = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        for(auto& el : structvec)
            counter += decider(el.color,temp);
    }
    std::chrono::duration<double> elapsedstruct = std::chrono::high_resolution_clock::now() - start;
    std::cout << "struct: " << elapsedstruct.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    ///////////////////

    //std::cout << "vecvec " << elapsedv.count() << "\n";
    std::cout << "ratio " << elapsedi/elapsedicond << "\n";

    //std::cout << "\n" << unsigned(charvec[100]) << "\n";
}