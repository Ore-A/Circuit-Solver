# Circuit Solver

## Project Overview
This project processes a CSV file defining a resistor network and outputs a CSV file detailing the currents within that network.

## Solving Methodology
The GNU Scientific Library (GSL) is used in the C program to solve linear systems of the form Ax = B. While loop detection in circuits is complex and often requires sophisticated algorithms, this project simplifies the process by annotating the input file with predefined loops. These loops assist in creating the voltage law equations necessary for solving the circuit. The input can have any number of loops, which can vary in length.

### Assumptions
- Only one voltage source is present, with two nodes labeled "V" representing the terminals of the power supply.
- Nodes are sequentially numbered starting from 1 with no missing integers.

## Input Format
The input is a CSV file with the following specifications:
- Two voltage nodes.
- Multiple resistors with specified resistances and directions of current flow.
- Loops for the voltage law indicated at the end of the file.

Example format:
```csv
# Type | Node 1 | Node 2 | Value
V,6,1,200
R,1,2,5
R,3,2,10
R,4,3,5
R,5,4,15
R,5,2,10
R,6,5,20
```
In this example:

- The first column indicates the type (R for resistor, V for voltage source, L for loop).
- The next columns define the nodes connected by the element and the resistance or voltage value.

## Output Format
The output is a CSV file similar to the input but with resistances replaced by currents, formatted to four decimal places. The loop rows may optionally be preserved. The output file is named `ans.csv`.

Example format:
```csv
# Type | Node 1 | Node 2 | Value
V,6,1,200
I,1,2,6.1538
I,3,2,-1.5385
I,4,3,-1.5385
I,5,4,-1.5385
I,5,2,-4.6154
I,6,5,-6.1538
```

## Instructions
To run the C program, execute the following commands in your terminal:

1. Compile the program using `make`:
   ```bash
   make
   ```
2. Run the program with the path to the input CSV file:
   ```bash
   ./kirchhoff_solve ./path/to/file.csv
   ```
Replace `./path/to/file.csv` with the actual path to your input CSV file.  

## Author

- Oreoluwa Abejide
