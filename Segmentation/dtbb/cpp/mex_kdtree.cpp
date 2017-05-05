#include "KdTree.h"
/*
 Constructs a kdtree through a mex-file call
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    plhs[0] = KdTree::createInMatlab(prhs); //[0],prhs[1],prhs[2]);
}

