#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "STInstance.hpp"
#include "InstanceReader.hpp"

InstanceReader InstanceReader::getObject() {
	static InstanceReader instance = InstanceReader();
    return instance;
}

InstanceReader::InstanceReader() {}

// These methods just make sure we don't create new copies of the Singleton
InstanceReader::InstanceReader(InstanceReader const& x) {}
void InstanceReader::operator=(InstanceReader const& x) {}

// Actual reader
STInstance InstanceReader::readSTInstance(std::string fileName) {
	std::ifstream inFile(fileName.c_str());

	if (!inFile.good()) throw std::invalid_argument("File " + fileName + " not found.");

	int dimension;
	int numEdges;

	// Read metadata
	inFile >> dimension >> numEdges;

	// Instantiate graph
	STInstance G = STInstance(fileName, dimension);

	int src, dest, cost;

	// Read edges
	while (numEdges--) {
		inFile >> src >> dest >> cost;
		G.setArcWeight(src, dest, cost);
	}

	int numTerminals, term;
	inFile >> numTerminals;

	while (numTerminals--) {
		inFile >> term;
		G.addTerminal(term);
	}

	return G;
}
