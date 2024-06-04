# Circuit Solver

## Project
The assignment is to produce a project that takes in a CSV file defining a resistor network and outputs a CSV file detailing the currents in that network. Several simplifications will be in-place as detailed in this section.

### Solving
You may use any numerical method of solving the linear system you choose. Compute all answers with double precision. For your convenience the GNU Scientific Library (GSL) is in- cluded along with an example C file showing how to solve linear systems in the form Ax = B but you may use either of your solvers from lab 11.
In a realistic setting, you would be required to identify the loops of the circuit yourself. However, since cycle detection is both well studied and yet difficult to properly implement, your input file will be annotated with the loops labeled to assist in creation of the voltage law equations, as demonstrated in the next section. Importantly, there can be any number of loops and the loops can be of any length (number of nodes).
Also, assume only one voltage source (i.e., there will be 2 nodes with a "V" label and they can be assumed to be the 2 terminals of a battery or power-supply for purposes of solving the circuit.) See the above worked example for how this is applied in practice.

### Input
A CSV file defining 2 voltage nodes and a number of resistors with values indicating resis- tance and the listed order indicating the "positive" direction of current flow through that edge. Therefore current flowing against that direction is "negative" like in the worked exam- ple. Importantly, the nodes will be numbered sequentially starting from 1 with no missing integers.
Also a number of rows indicating the loops for the voltage law will be included at the end of the file. As an example, the two loops from the above problem would be indicated by:

L,2,3,4,5

L,1,2,5,6

Where the end is assumed connected to the beginning node.

### Output
The output should be a near copy of the input with the types changed from resistance (in the form "R") to current with 4 decimal places (in the form "I" with a value of the form "X.XXXX") and preservation of the loop rows optional. This output should be saved to a file as directed in the grading section.

### Instructions
In order to run the C program, run the following two commands in bash, in order:

1. make
2. ./kirchhoff_solve ./path/to/file.csv

in the top level of the lab repository. Note that kirchhoff_solve is called with the path to the file as the only command-line argument.
