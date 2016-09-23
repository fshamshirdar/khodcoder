#include "utils.h"

double Utils::random(double low, double high)
{
    return (low + static_cast<double>(qrand()) / static_cast<double>(static_cast<double>(RAND_MAX) / (high - low)));
}

int Utils::random(int low, int high)
{
    return (qrand() % (high - low + 1)) + low;
}

