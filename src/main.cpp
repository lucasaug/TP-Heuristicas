#include <stdexcept>
#include <iostream>
#include <queue>
#include <string>
#include <chrono>

#include "InstanceReader.hpp"
#include "STInstance.hpp"

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
			STInstance G = reader.readSTInstance(file);

			Graph result;

			// We run terminal MST five times and get the average time
			int termMST;
			Time t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				termMST = G.termMST(result);
			}
			double timeSpentTermMST = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentTermMST /= 1000000000;

			// We run Steiner Leaf Removing MST five times and get the average time
			int removeStMST;
			t1 = timeNow();
			for (int i = 0; i < 5; i++) {
				removeStMST = G.removingSteinerLeafMST();
			}
			double timeSpentRemoveStMST = duration(timeNow() - t1) / 5.0;
			// convert time to seconds
			timeSpentRemoveStMST /= 1000000000;

			std::cout << G.getName() << "\n";
			//std::cout << "Terminal MST heuristic cost: ";
			//std::cout << termMST << "\n";
			//std::cout << "Elapsed time: ";
			std::cout << timeSpentTermMST << "\n";
			//std::cout << " seconds\n";
			//std::cout << "New MST heuristic cost: ";
			//std::cout << removeStMST << "\n";
			//std::cout << "Elapsed time: ";
			std::cout << timeSpentRemoveStMST << "\n";
			//std::cout << " seconds\n";
			std::cout << "\n";



		} catch (std::exception& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}

	return 0;
}
