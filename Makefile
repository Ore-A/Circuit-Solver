all: kirchhoff_solve

kirchhoff_solve: lab12.c
	gcc lab12.c -o kirchhoff_solve -lgsl -lgslcblas

clean:
	rm -rf kirchhoff_solve ans.csv
