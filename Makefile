all:
	g++ -g -c -o bin/Graph.o src/Graph.cpp
	g++ -g -c -o bin/STInstance.o src/STInstance.cpp
	g++ -g -c -o bin/InstanceReader.o src/InstanceReader.cpp
	g++ -g -c -o bin/Utils.o src/Utils.cpp
	g++ -g -c -o bin/Point.o src/Point.cpp
	g++ -g -std=c++11 -o bin/st src/main.cpp bin/Point.o bin/Graph.o bin/STInstance.o bin/InstanceReader.o bin/Utils.o
	rm bin/*.o

clean:
	rm bin/*
