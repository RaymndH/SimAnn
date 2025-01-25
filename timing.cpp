/// to do: find a way of generating random int_8s (for < )
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <unordered_map>


/// rng
static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}


static uint64_t s[4];

uint64_t next(void) {
	const uint64_t result = s[0] + s[3];

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result;
}

uint32_t bounded_rand(uint32_t x, uint32_t range) {
    uint64_t m = uint64_t(x) * uint64_t(range);
    return m >> 32;
}

/////////

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
template<typename T>
bool decider3(T x, T y){
 return x == x - ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1))); // max(x, y)
}

int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    for(int i = 0; i < 4; ++i){
        s[i] = rd();
    }

    int s = 1e5;
    struct site {
        int color;
    };
    std::vector<int> intvec(s, 0);
    std::vector<int> intvec2(s, 0);
    typedef uint8_t mytype;
    float temp = 1.9;
    float b = 1/temp;
    //typedef uint8_t mytype;
    std::vector<mytype> charvec(s, 0);
    std::vector<mytype> charvec2(s, 0);
    std::vector<std::vector<mytype>> vecvec(s/2, std::vector<mytype>(2,1));
    std::vector<site> structvec(s);
    for(auto& el : intvec)
        el = gen() % 5;
    for(auto& el : intvec2)
        el = gen() % 5;
    for(auto& el : charvec)
        el = gen() % 5;
    for(auto& el : charvec2)
        el = gen() % 5;
    for(auto& el : vecvec)
        for(auto& el1 : el)
            el1 = gen() % 5;
    for(auto& el : structvec)
        el.color = gen() % 5;
    typedef float exptype;
    std::vector<exptype> mymap;
    std::unordered_map<int, exptype> mymap2;

   
    
    std::cout << "\nmemory charvec = " << sizeof(charvec[0]) * charvec.size();
    std::cout << "\nmemory intvec = " << sizeof(intvec[0]) * intvec.size() << "\n";
    

    int counter = 0;
    int numreps = 1e4;
    int8_t charcounter = 0;
    float T = temp;


    float e = std::exp(1/200);
    for(int i = 0; i < 1000; ++i) {
        mymap2.insert({i,e});
        e *= std::exp(1/200);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {
        b = 1/T;  
        counter = 0;
        
    
        for(int j =0; j < s-1; ++j) {
            uint64_t v = next();
			uint32_t x,y;
			x = (uint32_t)((v & 0xFFFFFFFF00000000LL) >> 32);
            y = (uint32_t)(v & 0xFFFFFFFFLL);
            mytype alt = (charvec[j] + 1 + bounded_rand(x,4));
            float r = 0x1.0p-32 * y;
            if (alt >= 5) {
                alt -= 5;
            }
            if (alt <= charvec[j]){
            ++counter ;}
            else {
                //mytype a = 256 * std::exp( -b * (charvec2[j] - charvec[j])); replace with mapping
                auto de = alt - charvec[j];
                auto a = mymap[ b * de ];
                //std::cout << int(a) << " " << 256 * exp( -b * (charvec2[j] - charvec[j])) << "   ";
                if ( a > r) {
                    ++counter;
                }
            }
                }
        T-=(temp/numreps);
    }

    

    std::chrono::duration<double> elapsedc = std::chrono::high_resolution_clock::now() - start;

    std::cout << "char v char " << elapsedc.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    //////
    T = temp;
    start = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        b = 1/T;

        for(int j =0; j < s; ++j) {
            uint64_t v = next();
			uint32_t x,y;
			x = (uint32_t)((v & 0xFFFFFFFF00000000LL) >> 32);
            y = (uint32_t)(v & 0xFFFFFFFFLL);
            mytype alt = (charvec[j] + 1 + bounded_rand(x,4));
            float r = 0x1.0p-32 * y;
            if (alt >= 5) 
                alt -= 5;
            
            if (alt <= charvec[j]){
                ++counter ;
            }
            else  {
                auto de = alt - charvec[j];
                auto a = mymap2[ int(b * de) ];
                if ( a > r) {
                    ++counter;
                }
            }
                }
        T -= temp/numreps;
    }

    elapsedc = std::chrono::high_resolution_clock::now() - start;

    std::cout << "int v char " << elapsedc.count() << "\n";
    std::cout << "count: " << int(counter) << "\n\n";

    /////////////////////
    T = temp;
    start = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        b = 1/T;
        for(int j =0; j < s; ++j) {
            uint64_t v = next();
			uint32_t x,y;
			x = (uint32_t)((v & 0xFFFFFFFF00000000LL) >> 32);
            y = (uint32_t)(v & 0xFFFFFFFFLL);
            mytype alt = (charvec[j] + 1 + bounded_rand(x,4));
            float r = 0x1.0p-32 * y;
            if (alt >= 5)
                alt -= 5;
    
            if(alt<=intvec[j]){
                ++counter ;}
            else {
                float a = std::exp( - b * ( alt - intvec[j] ));
                //std::cout << "   " << a;
                //break;
                if(a > r){
                    ++counter;
                }
            }
                }
        T -= temp/numreps;
    }
    std::chrono::duration<double> elapsedi = std::chrono::high_resolution_clock::now() - start;
    std::cout << "int v int " << elapsedi.count() << "\n";
    std::cout << "count: " << counter << "\n\n";
    ///////////

    /*
    counter = 0;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < numreps; ++i) {    
        counter = 0;
        for(auto& el : intvec)
            counter += decider2(el,temp);
    }
    std::chrono::duration<double> elapsedicond = std::chrono::high_resolution_clock::now() - start;
    //std::cout << "int " << elapsedicond.count() << "\n";
    //std::cout << "count: " << counter << "\n\n";
    /////////////
    */
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
    /*
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
    */
    std::cout << "ratio " << elapsedi/elapsedc << "\n";
    

    //std::cout << "\n" << unsigned(charvec[100]) << "\n";
}