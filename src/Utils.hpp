#include <vector>

#ifndef TI_UTILS
#define TI_UTILS

/**
 * Useful functions for the code
 */
class Utils {
    private:
    Utils();

    public:
    static void  swapPieces(std::vector<int>&, int, int, int, int);
    static int   randomInt(int, int);
    static float randomFloat(float, float);
};

#endif
