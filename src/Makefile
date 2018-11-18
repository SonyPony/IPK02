CXXFLAGS=-std=c++14

all: ipk-mtrip

ipk-mtrip: main.o meter.o reflect.o probe.o connection.o argumentparser.o
	$(CXX) -o $@ $^

pack:
	zip xnguye16.zip Makefile *.h *.cpp dokumentace.pdf readme
