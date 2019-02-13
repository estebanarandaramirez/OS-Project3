all: phase1_project3
phase1_project3 : phase1_project3.c
	#gcc -g -Wall phase1_project3.c -o phase1_project3 -lm
	gcc phase1_project3.c -lm -o phase1_project3
	#gcc -o phase1_project3 phase1_project3.c -lm 

clean:
	rm -f phase1_project3
