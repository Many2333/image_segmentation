/*
// KdTree code, essentially a skimmed version of the implementation by Ihler et al. 
Main difference is that we use a predetermined tesselation of the 2D space while
their code was computing the range/center parameters of the KDtree on the fly.
Our code is faster, but less generic (works OK only for dense features)
 
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/

#include <utility>
#include <map>
#include "KdTree.h"

const char* KdTree::FIELD_NAMES[] = {"D", "N", "centers", "ranges", "leftch", "rightch","d0","d2"};
const int   KdTree::nfields = 8;

// Given the leaves, build the rest of the tree from the top down.
// Split the leaves along the most spread coordinate, build two balls
// out of those, and then build a ball around those two children.

uint32_t KdTree::split_tree(Tr_idx root,const Tr_idx* corner, const Tr_idx* dims,const Tr_idx *imsize,Tr_idx level,\
        const float fct,const float h0, const float v0)
{
    // decide whether to split horizontally or vertically: pick largest 
    int coord = (int) (dims[1]>dims[0]);
    if (dims[coord]==1) // largest size is 1: we have reached finest granularity 
    {
        // check if the point is within the image domain
        if ((corner[0]<=imsize[0])&&(corner[1]<=imsize[1])) {
            // compute index of leaf center for point @ [corner[0],corner[1]]
            // (horizontal - 1)*size_vertical + vertical;
            Tr_idx idx_point = (corner[0]-1)*imsize[1] + (corner[1] -1) + sh2*sv2;
            
            left_child[idx_point]  = idx_point;
            right_child[idx_point] = NO_CHILD;
            return idx_point;
        }
        else return NO_CHILD;
    }
    
    int r2 = root*2; const float offsets[2] = {h0,v0};
    for (int it=0;it<2;it++)
    {
        // range of node along dimension 'it':
        // borders will be {cen[it] - ranges[it], cen[it] + ranges[it]}
        // i.e. their length is dims[it] = 2*ranges[it] + 1
        const float ran   = ((float) dims[it]-1)/(fct*2.);
        ranges[r2  +it ]  = ran;
        centers[r2 +it ]  = ((float) corner[it]  + offsets[it])/fct  + ran;
    }
    
    Tr_idx half_size  = dims[coord]/2;
    Tr_idx corner_new[2], dims_new[2];
    for (int it = 0;it<2;it++)
    {
        corner_new[it] = corner[it];
        dims_new[it]   = dims[it];
    }
    dims_new[coord] = half_size;
    
    Tr_idx left_idx,right_idx;
    left_idx  = 2*root + 1;
    level     = level+1;
    
    left_idx   = split_tree(left_idx, corner_new, dims_new, imsize, level,fct,h0,v0);
    
    right_idx  = left_idx+1;
    const Tr_idx split = corner[coord] + half_size;
    
    if (split<=imsize[coord])
    {
        corner_new[coord] = split;
        right_idx  = split_tree(right_idx, corner_new, dims_new, imsize, level,fct,h0,v0);
    }
    else
        right_idx = NO_CHILD;
    
    left_child[root]   = left_idx;
    right_child[root]  = right_idx;
    calcStats(root,(right_idx  == NO_CHILD));
    return root;
}

void KdTree::calcStats(KdTree::index root,bool only_left)
{
}

// Public method to build the tree, just calls the private method with
// the proper starting arguments.
void KdTree::buildTree(const float fct,const float ho,const float vo)
{
  // set the range to equal zero at the leaves  
  Tr_idx i,j;
  int num_points2 = sh2*sv2;
  for (j=0, i=num_points2; j<num_points; i++,j++) {
      const int idims = i*dims;
      for(index k=0; k<dims; k++)
          ranges[idims+k] = 0;
      left_child[i]  = i;
      right_child[i] = NO_CHILD;
  }
  // top-left and bottom-right corners of the domain that is being split
  uint corner[2]  = {1,1};
  uint dims[2]    = {sh2,sv2}; 
  uint size[2]    = {sh0,sv0};
  split_tree(root(),corner, dims,size,0,fct,ho,vo);
}

/////////////////////////////// matlab functions ////////////////////////////

// Load the arrays already allocated in matlab from the given
// structure.
KdTree::KdTree(const mxArray* structure) 
{
  dims       = (unsigned int) mxGetScalar(mxGetField(structure,0,"D")); // get the dimensions
  num_points = (KdTree::index) mxGetScalar(mxGetField(structure,0,"N")); //
  
  centers = (float*) mxGetPr(mxGetField(structure,0,"centers"));
  ranges  = (float*) mxGetPr(mxGetField(structure,0,"ranges"));
  
  float *dims;
  dims = (float*) mxGetPr(mxGetField(structure,0,"d0"));
  sh0  = dims[0];
  sv0  = dims[1];
  dims = (float*) mxGetPr(mxGetField(structure,0,"d2"));
  sh2  = dims[0];
  sv2  = dims[1];
  
  left_child  = (KdTree::index*) mxGetData(mxGetField(structure,0,"leftch"));
  right_child = (KdTree::index*) mxGetData(mxGetField(structure,0,"rightch"));
}

// Create new matlab arrays and put them in the given structure.
mxArray* KdTree::createInMatlab(Mat_in *prhs)
{
    
  Mat_in _pointsMatrix = prhs[0];
  Mat_in _dims0        = prhs[1];
  Mat_in _dims2        = prhs[2];
  float h0             = mxGetScalar(prhs[3]);
  float v0             = mxGetScalar(prhs[4]);
  
  mxArray* structure;
  KdTree::index i, j;
  float *_points;
  
  // get fields from input arguments
  unsigned int Nd   = mxGetM(_pointsMatrix);
  KdTree::index Np  = mxGetN(_pointsMatrix);
  
  _points  = (float*)mxGetData(_pointsMatrix); 

  // dimensions of image 
  double *pdim2  = mxGetPr(_dims2);
  // number of points in image 
  unsigned int Np2   = (int) (pdim2[0]*pdim2[1]);
  
  // create structure, populate it, and get handles to the arrays
  structure = mxCreateStructMatrix(1, 1, nfields, FIELD_NAMES);
  
  mxSetField(structure, 0, "D",       mxCreateDoubleScalar((double) Nd));
  mxSetField(structure, 0, "N",       mxCreateDoubleScalar((double) Np));

  mwSize dims0[2]  = {1,2}; // dimensions of 'd0/d2'            (size) arrays
  mwSize dims[2];           // dimensions of 'centers/ranges'   (geometry) arrays
  mwSize dims1[1];          // dimensions of 'lenght/right'     (tree) arrays
  
  dims[0]  = Nd;
  dims[1]  = Np + Np2;
  dims1[0] = Np + Np2;
  
  
  // fields to be determined through call to tree constructor 
  mxSetField(structure, 0, "ranges",  mxCreateNumericArray(2,dims,mxSINGLE_CLASS, mxREAL));  
  mxSetField(structure, 0, "leftch",  mxCreateNumericMatrix(1, Np + Np2, mxUINT32_CLASS, mxREAL));
  mxSetField(structure, 0, "rightch", mxCreateNumericMatrix(1, Np + Np2, mxUINT32_CLASS, mxREAL));

  float* d;
  // allocate & populate structure fields
  mxSetField(structure, 0, "d0",      mxCreateNumericArray(2,dims0,mxSINGLE_CLASS, mxREAL));  
  d = (float *) mxGetData(mxGetField(structure, 0, "d0"));
  // dimensions of (power-of-2) kdtree
  double *pdim0 = mxGetPr(_dims0);
  d[0] = (float) pdim0[0];  d[1] = (float) pdim0[1];
  
  mxSetField(structure, 0, "d2",      mxCreateNumericArray(2,dims0,mxSINGLE_CLASS, mxREAL));
  d = (float *) mxGetData(mxGetField(structure, 0, "d2"));
  d[0] = (float) pdim2[0];  d[1] = (float) pdim2[1];
  
  mxSetField(structure, 0, "centers", mxCreateNumericArray(2,dims,mxSINGLE_CLASS, mxREAL));
  d = (float *) mxGetData(mxGetField(structure, 0, "centers"));
  // centers: grid of points provided through call
  for (j=0,i=Nd*Np2; j<Nd*Np; i++,j++)
    d[i] = _points[j];
  
  KdTree bt(structure); 
  bt.buildTree((float) 1,h0,v0);
  return structure;
}

