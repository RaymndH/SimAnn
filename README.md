# Simulated Annealing for Graph Coloring
Code to run simulated annealing on a graph of arbitrary size and connectivity c.
Graphs can be generated by editing and compiling plantstate.cpp
to compile use 
g++ testcode.cpp -std=c++11 -O3 -o testcode.out

to use plantstate:
g++ plantstate.cpp -std=c++11 -O3 -o plant
## Algorithm
![SAdiagram](https://github.com/user-attachments/assets/0ff19de0-f4eb-4bfc-88ae-fbe893a4b263)
.

A color change which changes the energy from 0 to +1
