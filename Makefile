all: phase1_project3
mc0 : mc0.c
	gcc -g -Wall phase1_project3.c -o phase1_project3

clean:
	rm -f phase1_project3
