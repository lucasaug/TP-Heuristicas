#include <stdexcept>
#include <iostream>
#include <queue>
#include <string>
#include <chrono>

#include "InstanceReader.hpp"
#include "TSPInstance.hpp"

// Time measurement definitions
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
typedef std::chrono::high_resolution_clock::time_point Time;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usage: tsp file1 [file2]...\n";
		return 1;
	}

	std::queue<std::string> files;

	for (int i = 1; i < argc; i++) {
		files.push(std::string(argv[i]));
	}

	while (!files.empty()) {
		std::string file = files.front();
		files.pop();

		InstanceReader reader = InstanceReader::getObject();
		try {
			TSPInstance G = reader.readTSPInstance(file);

			int treeLower = G.get1TreeLowerBound();
			int mstLower = G.getMSTLowerBound();

			std::vector<int> result;

			// We run NN five times and get the average time
			int NN;
			Time t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				NN = G.nearestNeighbor(result);
			}
			double timeSpentNN = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentNN /= 1000000000;

			// We run double-tree five times and get the average time
			int DT;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				DT = G.doubleMST(result);
			}
			double timeSpentDT = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentDT /= 1000000000;

			std::cout << G.getName() << ":\n";
			std::cout << "1-Tree lower bound: " << treeLower << "\n";
			std::cout << "Minimum spanning tree lower bound: " << mstLower << "\n";
			std::cout << "Nearest neighbor heuristic cost: " << NN << "\n";
			std::cout << "Elapsed time: " << timeSpentNN << " seconds\n";
			std::cout << "Double Minimum Spanning Tree heuristic cost: " << DT << "\n";
			std::cout << "Elapsed time: " << timeSpentDT << " seconds\n";
			std::cout << "\n";

			// We run 2-Opt local search five times and get the average time
			int LS2;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				// This is the call we actually use as a starting solution
				NN = G.nearestNeighbor(result);

				LS2 = G.twoOptSearch(result, NN);
			}
			double timeSpentLS2 = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentLS2 /= 1000000000;

			// We run 3-Opt local search five times and get the average time
			int LS3;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				// This is the call we actually use as a starting solution
				NN = G.nearestNeighbor(result);

				LS3 = G.threeOptSearch(result, NN, 0);
			}
			double timeSpentLS3 = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentLS3 /= 1000000000;

			// We run Variable Neighborhood Descent five times and get the average time
			int VND;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				// This is the call we actually use as a starting solution
				NN = G.nearestNeighbor(result);

				VND = G.VND(result, NN);
			}
			double timeSpentVND = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentVND /= 1000000000;

			// We run Simulated Annealing five times and get the average time
			int SA = -1;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				
				// This is the call we actually use as a starting solution
				NN = G.nearestNeighbor(result);

				const int maxIter = 1400;
				const float freezingTemp = 0.5;
				const float alpha = 0.01;

				int currSA = G.SA(result, NN, alpha, maxIter, NN, freezingTemp);
				if (SA == -1 || currSA < SA) {
					SA = currSA;
				}
			}
			double timeSpentSA = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentSA /= 1000000000;

			std::cout << "2-Opt local search: " << LS2 << "\n";
			std::cout << "Elapsed time: " << timeSpentLS2 << " seconds\n";
			std::cout << "3-Opt local search: " << LS3 << "\n";
			std::cout << "Elapsed time: " << timeSpentLS3 << " seconds\n";
			std::cout << "Variable Neighborhood Descent: " << VND << "\n";
			std::cout << "Elapsed time: " << timeSpentVND << " seconds\n";
			std::cout << "Simulated Annealing: " << SA << "\n";
			std::cout << "Elapsed time: " << timeSpentSA << " seconds\n";
			std::cout << "*\n*\n";

		} catch (std::exception& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	return 0;
}
