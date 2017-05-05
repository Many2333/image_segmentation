#ifndef __BALL_TREE_H
#define __BALL_TREE_H
/*
// KdTree code, essentially a skimmed version of the implementation by Ihler et al. 
Main difference is that we use a predetermined tesselation of the 2D space while
their code was computing the range/center parameters of the KDtree on the fly.
Our code is faster, but less generic (works OK only for dense features)
 
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/

#include <cassert>
#include <math.h>

#if (defined _WIN32 || defined _WIN64)  
typedef __int32 int32_t; 
typedef unsigned __int32 uint32_t; 
typedef __int64 int64_t; 
typedef unsigned __int64 uint64_t;  
#else 
#include <stdint.h> 
#endif 
#include "mex.h"
#include "defs.h"

float log(float);
float exp(float);
float sqrt(float);
float pow(float , float);

typedef const mxArray* Mat_in;
typedef unsigned int uint; 
class KdTree{
 public:
  typedef uint32_t index;              // define "index" type (long)
  typedef KdTree::index Tr_idx;
  
  /////////////////////////////
  // Constructors
  /////////////////////////////

  KdTree();
  KdTree(Mat_in structure);     // for loading ball trees from matlab
  ~KdTree(){};
  
  // For creating KdTree structures in matlab:
  static mxArray*  createInMatlab(Mat_in *);//pts,Mat_in dims0,Mat_in dims2);
  
  /////////////////////////////
  // Accessor Functions  
  /////////////////////////////
  Tr_idx root()							const { return 0; }  
  const float* center(Tr_idx i)			const { return centers+i*dims; }
  const float* range(Tr_idx i)			const { return ranges +i*dims; }
  Tr_idx left(Tr_idx i)                 const { return left_child[i]; }
  Tr_idx right(Tr_idx i)                const { return right_child[i]; }
  
  /////////////////////////////
  // Private class f'ns
  /////////////////////////////
 
  //static mxArray*  matlabMakeStruct(Mat_in *); //pts,Mat_in dims0,Mat_in dims2);
  void buildTree(const float fct,const float h0,const float v0);
  uint32_t split_tree(Tr_idx ,const Tr_idx* , const Tr_idx* ,const Tr_idx *,Tr_idx,const float,const float,const float);
  
  virtual void calcStats(Tr_idx,bool);     // construction recursion

  unsigned int dims;             // dimension of data 
  Tr_idx num_points;            // # of points 
  unsigned int sh0,sh2,sv0,sv2;
  float *centers;                // ball centers, dims numbers per ball 
  float *ranges;                 // bounding box ranges, dims per ball, dist from center to one side
  Tr_idx *left_child,  *right_child;  // left, right children; no parent indices
  
  static const char *FIELD_NAMES[];            // list of matlab structure fields
  static const int  nfields;
};
typedef KdTree::index Tr_idx;
const static Tr_idx NO_CHILD = (Tr_idx) -1;  // indicates no further children
#endif
