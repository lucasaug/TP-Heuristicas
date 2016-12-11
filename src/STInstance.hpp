#include <vector>
#include <string>

#include "Graph.hpp"

#ifndef TI_STInstance
#define TI_STInstance

/**
 * Enum to identify edge distance types
 */
enum Dist {
	EUC_2D,
	ATT
};

/**
 * This class describes an instance for the Steiner Tree
 */
class STInstance {
	private:
	std::string name;
	std::vector<int> terminals;

	public:
	Graph graph;
	STInstance(std::string, int);

	void        setName(std::string);
	std::string getName();

	void setDimension(int);
	int  getDimension();

	void setTerminals(std::vector<int>);
	void addTerminal(int);
	std::vector<int> getTerminals();

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

	// Steiner Heuristics
	int termMST(Graph&);
	int removingSteinerLeafMST();
};

#endif
