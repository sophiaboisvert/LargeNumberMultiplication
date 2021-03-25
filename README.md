Sophia Boisvert
Project 1
CS 4348.006

This program multiplies two large numbers (max size 256), read in from 
input.txt using threads, the number of which specified in input.txt
to a product (max size 512).

compile program and run using 
	g++ -o multiply -lpthread numMult.cpp
	./multiply

requires input.txt
	three lines. The first line is the number of threads, the 
	next two are the numbers to multiply
produces output.txt containing the result

test.sh is a testing script to compare the inputs running 12345678^2
one hundred times to the actual output 152415765279684.
