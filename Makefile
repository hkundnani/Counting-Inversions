CC=g++

all: preprocess run_preprocess query run_query

preprocess: preprocess.cpp
	$(CC) -std=c++11 -o preprocess preprocess.cpp

query: query.cpp
	$(CC) -std=c++11 -o query query.cpp

run_preprocess: preprocess
	time ./preprocess < input.txt

run_query: query
	time ./query < query.txt
