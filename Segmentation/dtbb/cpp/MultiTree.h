/*
 Header for `MultiTree' code.
 By MultiTree we mean a KDtree with a vector-valued entry per node:
We start with an image with a vector of values per pixel (e.g. the unary term scores for P parts)
The tesselation of the image domain will be commmon across parts.
What changes per part is the maximum/minimum of the part score within an interval.
So we use a single KDtree for all parts, and store a vector with the minima/maxima per part

 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/
#include <vector>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include "KdTree.h"

#ifndef __BALL_TREE_DENSITY_H
#define __BALL_TREE_DENSITY_H

inline float fastabs(float f) {
    return (f > 0) ? f : (-f);
}

class MultiTree : public KdTree {
  /////////////////////////////
  // Constructors
  /////////////////////////////
public:
  MultiTree() : KdTree() {bandwidth =  NULL; offset = NULL;}
  MultiTree(Mat_in structure);
  static mxArray*  createInMatlab(Mat_in, Mat_in,  Mat_in, Mat_in, Mat_in, Mat_in, int,float,float,float,int);
  bool   maxminDist(Tr_idx  dRoot,     Tr_idx  part,const KdTree& atTree, Tr_idx  aRoot, float *vals) const;
  bool   check_inside(Tr_idx  dRoot,     Tr_idx  part,const KdTree& atTree, Tr_idx  aRoot) const;
  float  sinDist(Tr_idx  dRoot,     Tr_idx  part,const KdTree& atTree, Tr_idx  aRoot) const;

  void   l2Dist(const float* loc, const Tr_idx dRoot, float *bounds) const;
  void   l2Dist(const float* loc, const Tr_idx dRoot, float *bounds,float,float,float,float ) const;
      
  float*  weight_all_u(Tr_idx  i)           { return weight + i*nval3;          }
  float*  weight_all_b(Tr_idx  i)           { return weight + i*nval3 + nval;   }
  float*  weight_all_l(Tr_idx  i)           { return weight + i*nval3 + 2*nval; }
  float*  offset_all(Tr_idx  val)           const { return (offset+val*dims); }  
  float*  bw_all(Tr_idx  val)               const { return (bandwidth+val*dims);                }  
  float   weight_at_u(Tr_idx i,Tr_idx  val)	const { return *(weight +i*nval3          + val);   }  
  float   weight_at_b(Tr_idx i,Tr_idx  val)	const { return *(weight +i*nval3 + nval   + val);   }    
  float   weight_at_l(Tr_idx i,Tr_idx  val)	const { return *(weight +i*nval3 + nval*2 + val);   }  
    void calcStats(Tr_idx,bool);
  float *bandwidth; 
  float *weight;   
  float *offset;
  Tr_idx nval,nval3;       // number of parts sharing the same Multi-Tree
  int NW;                  // # of weights: 2 by default, 3 if we use lower-upper
};
using namespace std;
typedef std::vector<MultiTree>   Mstree;  // multi-scale multi-tree

#endif
