#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <ostream>

float ran1(long *idum);
inline float ran1(const int &min, const int &max,long *idum)
{return static_cast<float>((max - min)*ran1(idum) + min);}

#endif /* __UTILITIES_H */
