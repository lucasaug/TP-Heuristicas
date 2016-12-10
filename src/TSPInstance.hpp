#include <vector>
#include <string>

#include "Graph.hpp"

#ifndef TI_TSPINSTANCE
#define TI_TSPINSTANCE

/**
 * Enum to identify edge distance types
 */
enum Dist {
	EUC_2D,
	ATT
};

/**
 * This class describes an instance for the TSP
 */
class TSPInstance {
	private:
	std::string name;
	Dist dType;

	public:
	Graph graph;
	TSPInstance(std::string, int, Dist);

	void        setName(std::string);
	std::string getName();

	void setDimension(int);
	int  getDimension();

	void setDType(Dist);
	Dist getDType();

	void setArcWeight(int, int, int);
	int  getArcWeight(int, int);

	// Lower bound calculators
	int get1TreeLowerBound();
	int getMSTLowerBound();

	// Constructive heuristics
	int nearestNeighbor(std::vector<int>&);
	int doubleMST(std::vector<int>&);

	// Local searches
	int twoOptSearch(std::vector<int>&, int);
	int threeOptSearch(std::vector<int>&, int, int);

	// Metaheuristics
	int VND(std::vector<int>&, int);
	int SA(std::vector<int>&, int, float, int, float, float);

};

#endif
