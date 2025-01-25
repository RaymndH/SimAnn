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
#include <set>

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

double d_rand() {
	return (next64() >> 11) * 0x1.0p-53;
}

uint32_t murmur64(uint64_t h) {
  h ^= h >> 33;
  h *= 0xff51afd7ed558ccdL;
  h ^= h >> 33;
  h *= 0xc4ceb9fe1a85ec53L;
  h ^= h >> 33;
  return h;
}

float m_rand(uint64_t h) {
	uint64_t mh = murmur64(h);
	float r = mh / (1<<31);
	return r;
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


int randint(int q) {
	return(next64() % q);
}

float randfloat() {
	float f = (next64() >> 11) * 0x1.0p-53;
	//std::cout << f << "\n";
	return(f);
}

//////////// structs, functions on structs

typedef uint32_t count_type;

struct site_type {
		int color;
		std::vector<count_type> qcount;
		std::vector<count_type> neighbs;
	};

float calc_energy (std::vector<site_type> sites) {
		float en = 0;
		for(size_t i = 0; i < sites.size(); i++) {
			site_type site = sites[i];
			for(int k = 0; k < site.neighbs.size(); k++) {
				site_type neighb = sites[site.neighbs[k]];
				if ( (neighb.color == site.color) and (i > site.neighbs[k]) )
					en++;
			}
		}
		return en;
	}


int main() {
    
	std::random_device rd;
    s64[0] = rd();
    //s64[0] = 1;
	s64[1] = 2901;
	s64[2] = 12;
	s64[3] = 112332;

	s32[0] = rd();
	//s32[0] = 1;
    s32[1] = 2901;
	s32[2] = 12;
	s32[3] = 112332;
	typedef unsigned index_type;

	int q = 5;


	

	
	std::vector<site_type> sites;

	

	///////////////////// read file
	struct Link {
		index_type s0;
		index_type s1;
	};
	std::ifstream fin;
	fin.open("./graph/N-1e3_c-18.0_q-5", std::ios_base::in);
	if (!fin)
		throw std::runtime_error("cannot open file  to read lattice");
	int maxs = 0;
	std::vector<Link> links;
	links.reserve(1000);
	bool first_line = true;
		while (1) {
			if (first_line) {
				std::string line;
				getline(fin, line, '\n');
				first_line = false;
				continue;
			}

			int s0, s1;
			fin >> s0 >> s1 ;
			if (!fin) break;
			
			if (s0 < 0 || s1 < 0)
				throw std::runtime_error("negative spin index in file " );
			if (s0 == s1)
				throw std::runtime_error("self link not allowed");
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
	std::cout << "nsites " << nsites << std::endl;

	float dT = 5e-5;
	float T = 1.4;
	int iters = T/dT;
	sites.reserve(nsites);
	sites.resize(nsites);

	for (std::size_t i = 0; i < links.size(); ++i) {
		const auto link = links[i];
		
		sites[link.s0].neighbs.push_back(link.s1);

		sites[link.s1].neighbs.push_back(link.s0);
	
	}
	
	


	nsites = sites.size();

	std::cout << "nsites " << nsites << std::endl;

	///////////// initialize colors and qcounts

	for(int i = 0; i < nsites; i++)
		sites[i].color = bounded_rand(5);

	for(int i = 0; i < nsites; i++)
		for(int j = 0; j < 5; j++)
			sites[i].qcount.push_back(0);
	
	for(int i = 0; i < nsites; i++){
		for(int j = 0; j < sites[i].neighbs.size(); j++){
			sites[i].qcount[sites[sites[i].neighbs[j]].color]++;
		}
	}

	std::cout << "initial energy is " << calc_energy(sites) << "\n";

	////////// maximal independent set algorithm
	// this part can probably be optimized a bit more, but it only needs to run once to introduce parallelism
	std::vector<int> groups;
	std::vector<double> randoms;
	std::set<int> V;
	std::set<int> V2;
	std::set<int> I = {};
	std::set<int> W = {};

	for (int i = 0; i < nsites; i++) {
		randoms.push_back(d_rand());
		groups.push_back(-1);
		V.insert(i);
		V2.insert(i);
		
	}
	int color = 0;
	std::set<int>::iterator itr;
	std::set<int>::iterator itr2;

	auto start = std::chrono::high_resolution_clock::now();

	for(color = 0; color < 10; color++){
		I.clear();
		W.clear();
		V.insert(V2.begin(), V2.end());
		while(V.size()) {
			W.clear();
			for(itr = V.begin(); itr != V.end(); itr++) {
				bool f = 1;
				for(auto neighb : sites[*itr].neighbs) {
					if ( std::find(V.begin(), V.end(), neighb)!= V.end() && neighb < *itr) f = 0;
				}
				if (f) W.insert(*itr); //Let W be the set of vertices in V with no earlier neighbours (based on the fixed ordering);
			}
			I.insert(W.begin(), W.end()); //Add W to I;
			
			for(itr2 = W.begin(); itr2 != W.end(); itr2++) { //Remove from V the nodes in the set W and all their neighbours.
				V.erase(*itr2);
				for(auto neighb : sites[*itr2].neighbs)
					V.erase(neighb);
			}
		}
		for(itr = I.begin(); itr != I.end(); itr++){
			groups[*itr] = color;
		}
		for(itr = I.begin(); itr != I.end(); itr++){
			V2.erase(*itr);
		}}
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
	std::cout << "finished with presort"<<std::endl;
	std::cout << "elapsed time: " << elapsed.count() << std::endl;
	int maxgroups = 0;
	for(int i = 0; i < n; i++)
		if (groups[i] > maxgroups) maxgroups = groups[i];
	std::cout << "max no of groups " << maxgroups << std::endl
	<< "breakdown:\n";
	std::vector<int> breakdown;
	std::vector<std::vector<int>> vecgroups;
	for (int i = 0; i < maxgroups; i++) {
		breakdown.push_back(0);
		vecgroups.push_back({});
		for(int ii = 0; ii< n; ii++) {
			if (groups[ii] == i){
				breakdown[i]++;
				vecgroups[i].push_back(ii);
			}
		}
	} 
	for (int i = 0; i < breakdown.size(); i++) {
		std::cout << i << ": " << breakdown[i] << std::endl;
	}
	std::cout << *std::fflush << std::endl;
	

	std::cout << "collisions: " << std::endl;
	for(int groupind = 0; groupind < vecgroups.size(); groupind++) {
		std::vector<int> group = vecgroups[groupind];
		for(int i = 0; i < group.size(); i++){
			site_type site = sites[group[i]];
			for(auto neighb : site.neighbs) {
				if (std::find(group.begin(), group.end(), neighb) != group.end()) {
					std::cout << "collision in group " << groupind << " at site " << group[i] << " with neighbor " << neighb << std::endl;
				}
			}
		}
	}

	///////////////////////////////////
	
	
	std::vector<int> addlist = {};
	std::vector<int> sublist = {};
	//int sames = 1;
	std::vector<int> vecsites = {};

	
	//clear groupings
	/*
	vecgroups.clear();
	vecgroups.push_back({});
	for(size_t i = 0; i < nsites; i++){
		vecgroups[0].push_back(i);
	}
	*/
	vecsites.clear();
	for(size_t i = 0; i < nsites; i++)
		vecsites.push_back(i);
	std::cout << "beginning " << iters << " iterations" << std::endl;
	for(int ii = 0; ii < iters; ++ii ) {
	//	for(auto group : vecgroups) {
	//		for(auto i : group) {
			for(auto i : vecsites) {
				site_type& site = sites[i];
				int col = site.color;
				int alt = (col + (next64() % q-1) + 1);
				if (alt >= q) alt -= q;
				int de = site.qcount[alt] - site.qcount[col];
				if (de < 0) {
					for(int k = 0; k < site.neighbs.size(); k++) {
							site_type& neighb = sites[site.neighbs[k]];
							neighb.qcount[alt]++;
							neighb.qcount[col]--;
							//if ( neighb.color = alt ) addlist.append(neighb,alt);
							//if ( neighb.color = site.col) sublist.append(neighb,col);
							site.color = alt;
						}
				}
				if ( de >= 0 ) {
					float rng = randfloat();// replace with hash(iter << 32 | site)
					//float rng = m_rand((uint64_t)ii << 32 | i);
					if (std::exp(-1/T*de) > rng ) {
						for(int k = 0; k < site.neighbs.size(); k++) {
							site_type& neighb = sites[site.neighbs[k]];
							neighb.qcount[alt]++;
							neighb.qcount[col]--;
							//if ( neighb.color = alt ) addlist.append(neighb,alt);
							//if ( neighb.color = site.col) sublist.append(neighb,col);
							site.color = alt;
						}
					}
				}
			//sort addlist?
			//std::sort(addlist.begin(), addlist.end())

			/*

			for pair in addlist:
				site = sites[pair[0]];
				color = pair[1];
				site.qcount[color]++;
			//same for sublist
			for pair in sublist:
				site = sites[pair[0]];
				color = pair[1];
				site.q[color]--;

				*/
			
			}
		T-=dT;
		if (ii % 10000 == 0) std::cout << "\r" << ((ii*1000)/iters)*.001 << "% complete "  << std::flush ;
		if (T<=0) {break;}
	}

	end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "\n" << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

	std::cout << "final energy is " << calc_energy(sites) << "\n";

	std::cout << "final temp: " << T;

}
