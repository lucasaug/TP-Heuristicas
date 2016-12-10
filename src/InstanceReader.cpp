#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "Point.hpp"
#include "TSPInstance.hpp"
#include "InstanceReader.hpp"

// Utility to trim spaces from string
bool isSpace(char c) {
	return c == ' ';
}

InstanceReader InstanceReader::getObject() {
	static InstanceReader instance = InstanceReader();
    return instance;
}

InstanceReader::InstanceReader() {}

// These methods just make sure we don't create new copies of the Singleton
InstanceReader::InstanceReader(InstanceReader const& x) {}
void InstanceReader::operator=(InstanceReader const& x) {}

// Actual reader
TSPInstance InstanceReader::readTSPInstance(std::string fileName) {
	std::ifstream inFile(fileName.c_str());

	if (!inFile.good()) throw std::invalid_argument("File " + fileName + " not found.");

	std::string line, name;
	int beginCoords = 0;
	int dimension;
	Dist distType;
	std::vector<Point> coords;

	// Read metadata
	while (!beginCoords && std::getline(inFile, line)) {
		int sep = line.find(":");
		if (sep == std::string::npos) {
			beginCoords = 1;
		} else {
			std::string info    = line.substr(0, sep),
			            content = line.substr(sep + 1);

			info.erase(std::remove_if(info.begin(), info.end(), &isSpace), info.end());
			content.erase(std::remove_if(content.begin(), content.end(), &isSpace), content.end());
			if (info == "NAME") {
				name = content;
			} else if (info == "DIMENSION") {
				dimension = std::atoi(content.c_str());
			} else if (info == "EDGE_WEIGHT_TYPE") {
				if (content != "EUC_2D" && content != "ATT") throw std::logic_error("Unrecognized distance function.");
				distType = (content == "EUC_2D" ? EUC_2D : ATT);
			}
		}
	}

	// Instantiate graph
	TSPInstance G = TSPInstance(name, dimension, distType);
	coords = std::vector<Point>();

	int id;
	float x, y;

	// Read coordinates
	while (inFile >> id >> x >> y) {
		if (coords.size() == dimension) throw std::logic_error("Number of coordinates greater than informed dimension.");

		Point p(x, y);
		for (int i = 0; i < coords.size(); i++) {
			int distance;

			if (distType == EUC_2D) {
				distance = p.EUC_2D_distance(coords[i]);
			} else {
				distance = p.ATT_distance(coords[i]);
			}

			G.setArcWeight(i + 1, coords.size() + 1, distance);
		}

		coords.push_back(p);
	}

	if (coords.size() < dimension) throw std::logic_error("Number of coordinates lesser than informed dimension.");

	return G;
}
