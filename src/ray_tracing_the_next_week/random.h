#ifndef _RANDOM_H_
#define _RANDOM_H_ 1

#include <cstdlib>

inline double random_double() { return rand() / (RAND_MAX + 1.0); }

#endif // _RANDOM_H_

