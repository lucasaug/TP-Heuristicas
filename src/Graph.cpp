#include <climits>
#include <vector>
#include <stack>
#include <queue>
#include <utility>

#include "Graph.hpp"

Graph::Graph() {}

Graph::Graph(int d) {
	dimension = d;

	values = Matrix(dimension);
	for (int i = 0; i < dimension; i++) {
		values[i] = Row(dimension);
		for (int j = 0; j < dimension; j++) {
			values[i][j] = INT_MAX;
		}
	}
}

void Graph::operator= (const Graph& G) { 
	dimension = G.dimension;

	values = Matrix(dimension);
	for (int i = 0; i < dimension; i++) {
		values[i] = Row(dimension);
		for (int j = 0; j < dimension; j++) {
			values[i][j] = G.values[i][j];
		}
	}
}

void Graph::setDimension(int d) {
	dimension = d;
}

int Graph::getDimension() {
	return dimension;
}

void Graph::setArcWeight(int i, int j, int value) {
	values[i - 1][j - 1] = value;
	values[j - 1][i - 1] = value;
}

int Graph::getArcWeight(int i, int j) {
	return values[i - 1][j - 1];
}

// Returns a new graph which is a copy of the current graph minus
// the specified vertex
Graph Graph::remove(int vertex) {
	Graph result = Graph(dimension - 1);

	for (int i = 1; i < vertex; i++) {
		for (int j = 1; j < vertex; j++) {
			result.setArcWeight(i, j, getArcWeight(i, j));
		}
		for (int j = vertex + 1; j <= dimension; j++) {
			result.setArcWeight(i, j - 1, getArcWeight(i, j));
		}
	}

	for (int i = vertex + 1; i <= dimension; i++) {
		for (int j = 1; j < vertex; j++) {
			result.setArcWeight(i - 1, j, getArcWeight(i, j));
		}
		for (int j = vertex + 1; j <= dimension; j++) {
			result.setArcWeight(i - 1, j - 1, getArcWeight(i, j));
		}
	}

	return result;
}

int Graph::getEdgeSum() {
	int sum = 0;

	for (int i = 0; i < dimension; i++) {
		for (int j = i; j < dimension; j++) {
			if (values[i][j] < INT_MAX)
				sum += values[i][j];
		}
	}

	return sum;
}

int Graph::largestArcWeight() {
	int largest = 0;

	for (int i = 0; i < dimension; i++) {
		for (int j = i; j < dimension; j++) {
			if (values[i][j] <= INT_MAX && values[i][j] > largest)
				largest = values[i][j];
		}
	}

	return largest;
}

int Graph::degree(int v) {
    int sum = 0;
    for (int i = 0; i < dimension; i++) {
        if (values[v - 1][i] < INT_MAX) sum++;
    }

    return sum;
}

Graph Graph::primMST() {
	std::vector<int> vertices(dimension, INT_MAX),
	                 parent(dimension, -1),
	                 inMST(dimension, 0);

    std::priority_queue<WeightedVertex, std::vector<WeightedVertex>, std::greater<WeightedVertex> > pq;

    pq.push(WeightedVertex(0, 0));
    vertices[0] = 0;

    while (!pq.empty()) {
    	WeightedVertex e = pq.top();
    	pq.pop();

    	int vertex = e.second;
    	inMST[vertex] = 1;

    	for (int i = 0; i < dimension; i++) {
    		if (values[vertex][i] < INT_MAX && !inMST[i] && vertices[i] > values[vertex][i]) {
    			pq.push(WeightedVertex(values[vertex][i], i));
    			vertices[i] = values[vertex][i];
    			parent[i] = vertex;
    		}
    	}
    }

    Graph result = Graph(dimension);
    for (int i = 1; i < dimension; i++) {
    	result.setArcWeight(i + 1, parent[i] + 1, values[i][parent[i]]);
    }
    return result;
}

// Runs a depth first search and returns the vertices
// in the order they were visited
void Graph::DFS(std::vector<int>& result) {
	result = std::vector<int>(0);
	std::stack<int> next;
	std::vector<int> visited = std::vector<int>(getDimension() + 1, 0);

	visited[1] = true;
	next.push(1);

	while (!next.empty()) {
		int current = next.top();
		next.pop();

		result.push_back(current);

		// Now we select the next vertices in reverse order of distance from the
		// current vertex
	    std::priority_queue<WeightedVertex, std::vector<WeightedVertex>, std::less<WeightedVertex> > pq;


		for (int i = 1; i <= getDimension(); i++) {
			if (getArcWeight(current, i) < INT_MAX && !visited[i]) {
				pq.push(WeightedVertex(getArcWeight(current, i), i));
			}
		}

		while (!pq.empty()) {
			int i = pq.top().second;
			pq.pop();

			next.push(i);
			visited[i] = 1;
		}
	}
}
