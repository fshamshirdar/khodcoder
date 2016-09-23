#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <QObject>

#define	RAND_MAX	0x7fffffff

class Utils
{
public:
    static double random(double low = 0, double high = RAND_MAX);
    static int random(int low = 0, int high = RAND_MAX);
};

#endif // UTILS_H
