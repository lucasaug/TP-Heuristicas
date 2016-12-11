#include <vector>

#ifndef TI_GRAPH
#define TI_GRAPH

/**
 * Weighted vertex used by Prim and Tree Lower Bound algorithms
 */
typedef std::pair<int, int> WeightedVertex;

/**
 * Matrix typedef
 */
typedef std::vector<int> Row;
typedef std::vector<Row> Matrix;

/**
 * General undirected graph representation
 */
class Graph {
	private:
	int dimension;
	Matrix values;

	public:
	Graph();
	Graph(int);
	void operator=(const Graph&);

	void setDimension(int);
	int  getDimension();

	void setArcWeight(int, int, int);
	int  getArcWeight(int, int);

	Graph remove(int);
	int getEdgeSum();
	int largestArcWeight();
	int degree(int);

	Graph primMST();
	void DFS(std::vector<int>&);
};

#endif
