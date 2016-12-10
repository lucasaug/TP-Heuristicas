all:
	g++ -c -o bin/Point.o src/Point.cpp
	g++ -c -o bin/Graph.o src/Graph.cpp
	g++ -c -o bin/TSPInstance.o src/TSPInstance.cpp
	g++ -c -o bin/InstanceReader.o src/InstanceReader.cpp
	g++ -c -o bin/Utils.o src/Utils.cpp
	g++ -std=c++11 -o bin/tsp src/main.cpp bin/Point.o bin/Graph.o bin/TSPInstance.o bin/InstanceReader.o bin/Utils.o
	rm bin/*.o

clean:
	rm bin/*
