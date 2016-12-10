#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Utils.hpp"

// Swaps two sections of a vector
void Utils::swapPieces(std::vector<int>& v, int start1, int end1, int start2, int end2) {
    // Supposes start1 < end1 <= start2 < end2
    std::vector<int> temp1, temp2;
    std::copy(v.begin() + start1, v.begin() + end1, std::back_inserter(temp1));
    std::copy(v.begin() + start2, v.begin() + end2, std::back_inserter(temp2));
    std::copy(temp2.begin(), temp2.end(), v.begin() + start1);
    std::copy(temp1.begin(), temp1.end(), v.begin() + start1 + (end2 - start2));
}

// Returns a random integer
// Could be replaced to use a more reliable random number generation library
int Utils::randomInt(int begin, int end) {
    return rand() % (end - begin) + begin;
}

// Returns a random floating point number
// Could be replaced to use a more reliable random number generation library
float Utils::randomFloat(float begin, float end) {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
