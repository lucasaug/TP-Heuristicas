#include <stdexcept>
#include <algorithm>
#include <climits>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

#include "Graph.hpp"
#include "Utils.hpp"
#include "STInstance.hpp"

STInstance::STInstance(std::string name, int dimension) {
	setName(name);

	terminals = std::vector<int>();
	graph = Graph(dimension);
}

void STInstance::setName(std::string newName) {
	name = newName;
}

std::string STInstance::getName() {
	return name;
}

void STInstance::setDimension(int newDimension) {
	graph.setDimension(newDimension);
}

int STInstance::getDimension() {
	return graph.getDimension();
}

void STInstance::setTerminals(std::vector<int> nTerm) {
	terminals = nTerm;
}

void STInstance::addTerminal(int vertex) {
	terminals.push_back(vertex);
}

std::vector<int> STInstance::getTerminals() {
	return terminals;
}

void STInstance::setArcWeight(int i, int j, int value) {
	graph.setArcWeight(i, j, value);
}

int STInstance::getArcWeight(int i, int j) {
	return graph.getArcWeight(i, j);
}

// Calculates lower bound by removing a vertex, finding the
// Minimum Spanning Tree for the remaining graph, and then
// joins it with the leftover vertex using the two smallest
// edges that join the MST with this vertex
// The bound is the sum of the edges in the result
int STInstance::get1TreeLowerBound() {
	// Find sub-MST
	Graph sub = graph.remove(1);
	sub = sub.primMST();

    std::priority_queue<WeightedVertex, std::vector<WeightedVertex>, std::greater<WeightedVertex> > pq;
	// Sorts the edges from the cut between the MST and the first vertex
	for (int i = 2; i < getDimension(); i++) {
		pq.push(WeightedVertex(getArcWeight(1, i), i));
	}

	int result = pq.top().first;
	pq.pop();
	result += pq.top().first;
	result += sub.getEdgeSum();

	return result;
}

// Returns the sum of the edges of the minimum spanning tree
// for the graph
int STInstance::getMSTLowerBound() {
	return graph.primMST().getEdgeSum();
}

// Nearest neighbor heuristic: starts with the cities in the smallest edge, then
// finds the closest city to the last one inserted, and moves there. This process is repeated
// until we visit every city. When that happens we return to the first city
int STInstance::nearestNeighbor(std::vector<int>& path) {
	int size = getDimension();
	std::vector<int> inPath = std::vector<int>(size + 1, 0);
	path = std::vector<int>();

	int x, y, weight;
	weight = INT_MAX;
	for (int i = 1; i <= size; i++) {
		for (int j = 1; j <= size; j++) {
			if (getArcWeight(i, j) < weight) {
				x = i;
				y = j;
				weight = getArcWeight(i, j);
			}
		}
	}

	path.push_back(x);
	path.push_back(y);
	inPath[x] = 1;
	inPath[y] = 1;

	int length = weight;

	while (path.size() < size) {
		int currentVertex = path.back(),
		    nextVertex = -1,
		    nextWeight = INT_MAX;

		for (int i = 1; i <= size; i++) {
			int dist = getArcWeight(currentVertex, i);

			if (dist < INT_MAX && !inPath[i] && dist < nextWeight) {
				nextVertex = i;
				nextWeight = dist;
			}
		}

		path.push_back(nextVertex);
		inPath[nextVertex] = 1;
		length += nextWeight;
	}

	length += getArcWeight(path[size - 1], path[0]);
	path.push_back(path[0]);

	return length;
}

// Generates the Minimum Spanning Tree for the graph, and then
// turns it into an Euler cycle. To finish, removes any repeated
// vertices from the path (takes a shortcut)
// Requires the triangle inequality to hold. Since pseudo-euclidean
// distance does not guarantee it, in this case we update the edges
// on the graph to make it hold, and in the end we subtract the needed
// amount
int STInstance::doubleMST(std::vector<int>& path) {
	Graph g = graph;

	int largest = 0;

	largest = graph.largestArcWeight();

	for (int i = 1; i <= getDimension(); i++) {
		//for (int j = 1; j <= getDimension(); j++) {
		for (int j = i + 1; j <= getDimension(); j++) {
			g.setArcWeight(i, j, getArcWeight(i, j) + largest);
		}
	}

	Graph mst = g.primMST();
	mst.DFS(path);

	int length = getArcWeight(path[getDimension() - 1], path[0]);
	for (int i = 1; i < path.size(); i++) {
		length += getArcWeight(path[i - 1], path[i]);
	}

	length -= getDimension() * largest;

	path.push_back(path[0]);

	return length;
}

// Runs a local search using the 2-Opt first-improving neighborhood
int STInstance::twoOptSearch(std::vector<int>& solution, int solutionValue) {

	int foundImprov = 1;
	while (foundImprov) {
		foundImprov = 0;

		for (int i = 0; i < getDimension() && !foundImprov; i++) {
			// If i == 0, we must not look at the last edge
			int limit = (i != 0) ? getDimension() : getDimension() - 1;
			for (int j = i + 2; j < limit && !foundImprov; j++) {
				int delta = getArcWeight(solution[i]    , solution[j])
						  + getArcWeight(solution[i + 1], solution[j + 1])
						  - getArcWeight(solution[i]    , solution[i + 1])
						  - getArcWeight(solution[j]    , solution[j + 1]);

				if (delta < 0) {
					// We found an improving neighbor
					foundImprov = 1;

					// Change the current solution
					std::reverse(solution.begin() + i + 1, solution.begin() + j + 1);

					// Update the solution cost
					solutionValue += delta;
				}
			}
		}
	}

	return solutionValue;
}

// Runs a local search using the 3-Opt first-improving neighborhood
// If singleIter == 1, only runs the search once: Used for VND
int STInstance::threeOptSearch(std::vector<int>& solution, int solutionValue, int singleIter) {

	int foundImprov = 1;
	while (foundImprov) {
		foundImprov = 0;

		for (int i = 0; i < getDimension() && !foundImprov; i++) {
			// If i == 0, we must not look at the last edge
			int limit = (i != 0) ? getDimension() : getDimension() - 1;

			for (int j = i + 2; j < limit && !foundImprov; j++) {

				for (int k = j + 2; k < limit && !foundImprov; k++) {

					// Now we test each of the 4 possibilities
					int delta1 = getArcWeight(solution[i]   , solution[j])
							  + getArcWeight(solution[i + 1], solution[k])
							  + getArcWeight(solution[j + 1], solution[k + 1])
							  - getArcWeight(solution[i]    , solution[i + 1])
							  - getArcWeight(solution[j]    , solution[j + 1])
							  - getArcWeight(solution[k]    , solution[k + 1]),
						delta2 = getArcWeight(solution[i]   , solution[j + 1])
							  + getArcWeight(solution[k]    , solution[j])
							  + getArcWeight(solution[i + 1], solution[k + 1])
							  - getArcWeight(solution[i]    , solution[i + 1])
							  - getArcWeight(solution[j]    , solution[j + 1])
							  - getArcWeight(solution[k]    , solution[k + 1]),
						delta3 = getArcWeight(solution[i]   , solution[k])
							  + getArcWeight(solution[j + 1], solution[i + 1])
							  + getArcWeight(solution[j]    , solution[k + 1])
							  - getArcWeight(solution[i]    , solution[i + 1])
							  - getArcWeight(solution[j]    , solution[j + 1])
							  - getArcWeight(solution[k]    , solution[k + 1]),
						delta4 = getArcWeight(solution[i]   , solution[j + 1])
							  + getArcWeight(solution[k]    , solution[i + 1])
							  + getArcWeight(solution[j]    , solution[k + 1])
							  - getArcWeight(solution[i]    , solution[i + 1])
							  - getArcWeight(solution[j]    , solution[j + 1])
							  - getArcWeight(solution[k]    , solution[k + 1]);

					if (delta1 < 0) {
						foundImprov = 1;

						// Change the current solution (A -> Brev -> Crev)
						std::reverse(solution.begin() + i + 1, solution.begin() + j + 1);
						std::reverse(solution.begin() + j + 1, solution.begin() + k + 1);

						// Update the solution cost
						solutionValue += delta1;
					} else if(delta2 < 0) {
						foundImprov = 1;

						// Change the current solution (A -> C -> Brev)
						std::reverse(solution.begin() + i + 1, solution.begin() + j + 1);
						Utils::swapPieces(solution, i + 1, j + 1, j + 1, k + 1);


						// Update the solution cost
						solutionValue += delta2;
					} else if(delta3 < 0) {
						foundImprov = 1;

						// Change the current solution (A -> Crev -> B)
						std::reverse(solution.begin() + j + 1, solution.begin() + k + 1);
						Utils::swapPieces(solution, i + 1, j + 1, j + 1, k + 1);

						// Update the solution cost
						solutionValue += delta3;
					} else if(delta4 < 0) {
						foundImprov = 1;

						// Change the current solution (A -> C -> B)
						Utils::swapPieces(solution, i + 1, j + 1, j + 1, k + 1);

						// Update the solution cost
						solutionValue += delta4;
					}
				}
			}
		}

		if (singleIter) break;
	}

	return solutionValue;
}

// Variable Neighborhood Descent selecting between 2-Opt and 3-Opt
int STInstance::VND(std::vector<int>& solution, int solutionValue) {

	int foundImprov = 1;
	while (foundImprov) {
		foundImprov = 0;
		// We run 2-Opt search from the current point
		solutionValue = twoOptSearch(solution, solutionValue);

		// After 2-Opt finds some local optimum, run 3-Opt
		int newVal = threeOptSearch(solution, solutionValue, 1);

		if (newVal != solutionValue) foundImprov = 1;
		solutionValue = newVal;
	}

	return solutionValue;
}

// Simulated Annealing generating random neighbors in the 2-Opt neighborhood
int STInstance::SA(std::vector<int>& solution, int solutionValue, float alpha,
	                int maxIter, float startingTemp, float freezingTemp) {
	std::vector<int> current = solution;
	int currentValue = solutionValue;

	float T = startingTemp;

	while (T > freezingTemp) {
		int numIter = 0;

		while (numIter < maxIter) {
			numIter++;

			// Generate random neighbor
			std::vector<int> randSolution = current;
			int randNeighCost = currentValue;

			// Define two edges to remove
			int i = Utils::randomInt(0, getDimension() - 2),
			    j = Utils::randomInt(i + 2, (i != 0) ? getDimension() : getDimension() - 1);


			randNeighCost += getArcWeight(current[i]    , current[j])
					      +  getArcWeight(current[i + 1], current[j + 1])
						  -  getArcWeight(current[i]    , current[i + 1])
						  -  getArcWeight(current[j]    , current[j + 1]);

			// Change the random solution
			std::reverse(randSolution.begin() + i + 1, randSolution.begin() + j + 1);

			int delta = randNeighCost - currentValue;
			if (delta < 0) {
				current      = randSolution;
				currentValue = randNeighCost;

				if (currentValue < solutionValue) {
					solution      = current;
					solutionValue = currentValue;
				}
			} else {
				float draw = Utils::randomFloat(0, 1);
				if (draw < std::exp(-1 * delta / T)) {
					current = randSolution;
					currentValue = randNeighCost;
				}
			}
		}

		T *= alpha;
		numIter = 0;
	}

	return solutionValue;
}

// Creates a Minimum Spanning Tree for the terminal vertices
int STInstance::termMST(Graph& result) {
	Graph temp = Graph(terminals.size());

	for (int i = 0; i < terminals.size(); i++) {
		for (int j = 0; j < terminals.size(); j++) {
			temp.setArcWeight(i + 1, j + 1, getArcWeight(terminals[i], terminals[j]));
		}
	}

	temp = temp.primMST();
	result = Graph(getDimension());

	for (int i = 0; i < terminals.size(); i++) {
		for (int j = 0; j < terminals.size(); j++) {
			result.setArcWeight(terminals[i], terminals[j], temp.getArcWeight(i + 1, j + 1));
		}
	}

	return temp.getEdgeSum();
}

// Creates a Minimum Spanning Tree for the whole graph and then removes
// Steiner node leaves
// This should return the resulting MST, but I'll leave it as an eternal TODO
int STInstance::removingSteinerLeafMST() {
	Graph temp = graph.primMST();
	int checkLeaves = 1;

	while (checkLeaves) {
		checkLeaves = 0;
		for (int i = 1; i <= temp.getDimension(); i++) {
			if (std::find(terminals.begin(), terminals.end(), i) == terminals.end() &&
				temp.degree(i) == 1) {
				temp = temp.remove(i);
				checkLeaves = 1;
			}
		}
	}

	return temp.getEdgeSum();
}
