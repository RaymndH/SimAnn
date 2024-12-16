#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <string>
#include <cassert>

////////////////////

static inline uint64_t rotl64(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t s64[4];

uint64_t next64(void) {
	const uint64_t result = s64[0] + s64[3];

	const uint64_t t = s64[1] << 17;

	s64[2] ^= s64[0];
	s64[3] ^= s64[1];
	s64[1] ^= s64[2];
	s64[0] ^= s64[3];

	s64[2] ^= t;

	s64[3] = rotl64(s64[3], 45);

	return result;
}

uint32_t bounded_rand(uint32_t range) {
    uint32_t x = next64();
    uint64_t m = uint64_t(x) * uint64_t(range);
    return m >> 32;
}

///////////////////////


static inline uint32_t rotl32(const uint32_t x, int k) {
	return (x << k) | (x >> (32 - k));
}


static uint32_t s32[4];

uint32_t next32(void) {
	const uint32_t result = s32[0] + s32[3];

	const uint32_t t = s32[1] << 9;

	s32[2] ^= s32[0];
	s32[3] ^= s32[1];
	s32[1] ^= s32[2];
	s32[0] ^= s32[3];

	s32[2] ^= t;

	s32[3] = rotl32(s32[3], 11);

	return result;
}
template<typename S>
float calc_energy(std::vector<S>& sites, uint32_t n) {
	float en = 0;
	for(uint32_t i = 0; i < n; i++) 
		for(int k = 0; k < sites[i].neighbs.size(); k++){
			if ( (sites[sites[i].neighbs[k]].color == sites[i].color) and (i > sites[i].neighbs[k]) )
				en++;}
	
	return en;
}



int main() {
    
	std::random_device rd;
    //s64[0] = rd();
    s64[0] = 1;
	s64[1] = 2901;
	s64[2] = 12;
	s64[3] = 112332;

	//s32[0] = rd();
	s32[0] = 1;
    s32[1] = 2901;
	s32[2] = 12;
	s32[3] = 112332;
	typedef unsigned index_type;

	int c = 0;
	
	int q = 5;


	typedef uint32_t count_type;

	struct site_type {
		int color;
		std::vector<count_type> qcount;
		std::vector<count_type> neighbs;
	};
	std::vector<site_type> sites;

	///////////////////// read file
	struct Link {
		index_type s0;
		index_type s1;
	};
	std::ifstream fin;
	fin.open("/Users/ray/anc/graph/N-1e3_c-17.0_q-5", std::ios_base::in);
	if (!fin)
		throw std::runtime_error("cannot open file  to read lattice");
	int maxs = 0;
	std::vector<Link> links;
	links.reserve(100);

	bool first_line = true;
		while (1) {
			if (first_line) {
				std::string line;
				getline(fin, line, '\n');
				first_line = false;
				continue;
			}

			int s0, s1;
			int trash;
			fin >> s0 >> s1 >> trash;
			if (!fin) break;
			
			if (s0 < 0 || s1 < 0)
				throw std::runtime_error("negative spin index in file " );

			links.push_back({ index_type(s0), index_type(s1)});

			maxs = s0 > maxs ? s0 : maxs;
			maxs = s1 > maxs ? s1 : maxs;
		}

		fin.close();

		int nsites = 0;

	
	
	std::vector<index_type> phys_sites(maxs + 1, index_type(-1));

		for (std::size_t i = 0; i < links.size(); ++i) {
			auto link = links[i];

			if (phys_sites[link.s0] == int(-1))
				link.s0 = phys_sites[link.s0] = nsites++;
			else
				link.s0 = phys_sites[link.s0];

			if (phys_sites[link.s1] == int(-1))
				link.s1 = phys_sites[link.s1] = nsites++;
 			else
				link.s1 = phys_sites[link.s1];
		}

	int n = nsites;
	float dT = 1e-5;
	float T = 1.1;
	int iters = T/dT;
	sites.reserve(nsites);
	sites.resize(nsites);

	for (std::size_t i = 0; i < links.size(); ++i) {
		const auto link = links[i];
		
		sites[link.s0].neighbs.push_back(link.s1);

		sites[link.s1].neighbs.push_back(link.s0);
	
	}
	std::cout << nsites << &std::fflush;
	


	nsites = sites.size();
	for(int i = 0; i < nsites; i++)
		sites[i].color = bounded_rand(5);
	for(int i = 0; i < nsites; i++)
		for(int j = 0; j < 5; j++)
			sites[i].qcount.push_back(0);
				
	

	std::cout << "qcount is" << sites[0].qcount[0];

	///////////////////// fill vectors randomly
	/*
	for(int i = 0; i < n; i++){
		sites[i].color = bounded_rand(5);
		int top = bounded_rand(25);
		int neighbct = 0;
		sites[i].qcount.reserve(5);
		for(auto qc : sites[i].qcount)
			qc = 0;
		while(neighbct < top){
			uint32_t x = bounded_rand(n);
			if ((x!=i)&&(std::find(sites[i].neighbs.begin(), sites[i].neighbs.end(), x) == sites[i].neighbs.end())) {
				sites[i].neighbs.push_back(x);
				neighbct++;
			}
		}
	}*/

	///////////////////////////// fill qcount
	for(int i = 0; i < nsites; i++){
		for(int j = 0; j < sites[i].neighbs.size(); j++){
			sites[i].qcount[sites[sites[i].neighbs[j]].color]++;
		}
	}

	////////////////////////////// diagnostics block 
	/*
	for(int i = 0; i < 5; i++){
		std::cout << "site " << i << " color : " << sites[i].color << "\n";
		for(int j = 0; j < sites[i].neighbs.size(); j++)
			std::cout << sites[sites[i].neighbs[j]].color << " ";
		std::cout << "\nqcount: ";
		for(int j = 0; j < 5; j++)
			std::cout << "" << static_cast<uint32_t>(sites[i].qcount[j]) << " ";
		
		std::cout << "\n";
	}*/

	////////////////////////////////////// calc energy
	float en = 0;
	std::cout << "\nsize is " << nsites << "\n";
	for(auto site& : sites) 
		for(int k = 0; k < sites[i].neighbs.size(); k++){
			//std::cout << "comparing " << i << " and " << sites[i].neighbs[k] << "\n";
			if ( (sites[sites[i].neighbs[k]].color == sites[i].color) and (i > sites[i].neighbs[k]) )
				en++;}
	std::cout << "initial energy is " << calc_energy(sites, n) << "\n";
	


	///////////////////////////////////
	int counter = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for(int ii = 0; ii < iters; ++ii ){
		
		for(auto site : sites){
			//site_type& site = sites[i];
			uint32_t rng32 = next32();
			uint16_t a16,b16;
			a16 = (rng32 & 0xFFFF0000) >> 16; //split 32 random bits into 2 16
			b16 = rng32 & 0x0000FFFF;
			uint32_t m = uint32_t(a16) * uint32_t(4);
			//int ocolor = sites[i].color;
			int ocolor = site.color;
			uint32_t altcolor = (sites[i].color+(m>>16)+1)%5;
			if(ocolor==altcolor) {std::cout<<"ERROR ";}
			int de = sites[i].qcount[altcolor] - sites[i].qcount[sites[i].color];
			m = (uint32_t(b16) * uint32_t(10000)) >> 16;
			uint32_t res = m;
			
			if (res < 10000*std::exp(-de/T)){
				for(int k = 0; k < sites[i].neighbs.size(); ++k){
					site_type& neighbor = sites[sites[i].neighbs[k]];
					if (neighbor.qcount[ocolor]==0) {std::cout << "\nneighb is" << site.neighbs[k] <<&std::fflush;}
					assert(neighbor.qcount[ocolor]!=0);
					--neighbor.qcount[sites[i].color];
					++neighbor.qcount[altcolor];
				}
				sites[i].color = altcolor;
			}
		}

		counter++;
		if(counter == 10000){
			std::cout << "\niteration "  << " out of " << iters;
			std::cout << "\nen: " << calc_energy(sites, n)<< "\n";
			
			if(calc_energy(sites, n)==-en)
				break;
			en = (calc_energy(sites, n));
			counter=0;
			std::chrono::duration<double> el = std::chrono::high_resolution_clock::now() - start;
			std::cout << "\ntime elapsed: " << el.count();
		}
		T-=dT;
		if (T<=0) {break;}
	}

	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

	std::cout << "final energy is " << calc_energy(sites, n) << "\n";

	std::cout << "final temp: " << T;

}
