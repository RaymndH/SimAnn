# Simulated Annealing for Graph Coloring
Code to run simulated annealing on a graph of arbitrary size and connectivity c.
Graphs can be generated by editing and compiling plantstate.cpp
to compile use 
g++ testcode.cpp -std=c++11 -O3 -o testcode.out

to use plantstate:
g++ plantstate.cpp -std=c++11 -O3 -o plant
## Algorithm
The basics of the algorithm is to take a undirected graph and find a coloring which satisfies the fewest possible color conflicts between connected nodes given a maximum number of colors.

The basis of this is to start with a random coloring, and then iterate through each node. At each node, the color is changed to an alternative number, and then the number of conflicts is calculated. If the new coloring has the same or fewer conflicts, then it is accepted. If the new coloring has more conflicts, then it is conditionally accepted with a probability proportional to exp(-de/T), where T is a temperature which starts high and decreases each iteration.

![SAdiagram](https://github.com/user-attachments/assets/4c4fb895-bc55-4ce5-8b30-8b887706a060)
.

Example of a graph with 4 nodes, and 3 colors. Changing the color of the third node from green to red changes the energy of the system from 0 to +1. This change will be accepted with a probability of exp(-1/T).


