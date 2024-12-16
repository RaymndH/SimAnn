#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>
#include <string>
#include <fstream>
#include <functional>
#include <cmath>



int main() {
    // Write C++ code here
    std::mt19937 generator;
    std::function<double()> rng_;

    generator.seed(std::time(nullptr));
    rng_ = std::bind(std::uniform_real_distribution<double>(0, 1), std::ref(generator));
    //auto rng_ = [generator](void) {return double(generator()/generator.max());};
    int sig = 4;
    int mant = 1;
    int n = int(sig*std::pow(10,mant));
    float c = 18.0;
    int q = 5;
    float m =c*n/2;
    int i =0;




    std::string fname;
    fname = "N-" + std::to_string(sig) + "e" + std::to_string(mant) + "_c-" + std::to_string( int(c) ) + "." + std::to_string(int(10*c)%(10*int(c))) +  "_q-" + std::to_string(q);

    std::vector<int> plant;
    std::vector<std::set<int> > adj;
    std::set<int>::iterator it;
    plant.resize(n);
    adj.resize(n);
    std::cout << "plant:\n";
    std::ofstream pfile;
    //fname = "1";
    pfile.open("plant/"+fname+".txt", std::ofstream::out | std::ofstream::trunc);
    pfile.close();
    pfile.open("plant/"+fname+".txt",std::ios_base::app);
    for(int j=0; j<n; ++j)
        {
            plant[j] = int(q * rng_());
            pfile << plant[j] << "\n";
        }
    pfile.close();
    int a, b;
    
    std::cout << fname;
    std::ofstream efile;
    efile.open("graph/"+fname, std::ofstream::out | std::ofstream::trunc);
    efile.close();
    efile.open("graph/"+fname,std::ios_base::app);

  
    
    efile << "N = " << n << ", c = " << c << ", q = " << q << "\n";
    while(i < m){
        a=int(rng_()*n);
        b=int(rng_()*n);
        if(plant[a]!=plant[b]) // a and b are different colors
        {
            if(adj[a].find(b)==adj[a].end()) // a and b are not connected
            {
                adj[a].insert(b); // add edge to both
                adj[b].insert(a);
                efile << a << " " << b << " " << "\n"; 
                i++;

            
            }
        }

    }

    for(int j=0; j<3; ++j){
        std::cout << "\n"<< adj[j].size() << " neighbs of " << j <<": ";
        
        for(it=adj[j].begin(); it!=adj[j].end(); ++it)
            std::cout << *it << " ";

    }
    int e = 0;
    
    
    

    
    efile.close();
    std::cout << adj.size() << " sites" ;
    return 0;
}