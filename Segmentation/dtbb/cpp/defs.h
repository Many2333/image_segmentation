#ifndef DEFS
#define DEFS
/*
 * Some common definitions 
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
 */
#define my_min(a, b) ((a)<(b)? (a):(b))
#define my_max(a, b) ((a)>(b)? (a):(b))

#include <vector>
typedef std::vector<float> vec;
typedef std::vector<vec>   arr;
typedef std::vector<arr>   arr3;
typedef std::vector<std::vector<int> > cnfs;

#define FALSE 0
#define TRUE 1
#define PI 3.141592653589
#endif