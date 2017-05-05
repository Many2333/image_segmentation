#include "MultiTree.h"
/* 
  Constructs a multitree through a mex-file call
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const  mxArray *prhs[]) {
     int   Noffsets = (int) mxGetN(prhs[2]);
     int   NW       = (int) mxGetScalar(prhs[6]); 
     float fct = (float) mxGetScalar(prhs[7]); 
     float offh = (float) mxGetScalar(prhs[8]); 
     float offv = (float) mxGetScalar(prhs[9]); 
     
     plhs[0] = MultiTree::createInMatlab(prhs[0],prhs[1],prhs[2],prhs[3],prhs[4],prhs[5],NW,fct,offh,offv,Noffsets);
}
