#include "MultiTree.h"
        
float *pMin,  *pMax;                // need to declare these here, for kernel
float **pAdd, *pErr;
float *min,   *max;                  // derivative functions in kernel.h
float globalLowerBound;
/*
// void MultiTree::maxminDist(KdTree::index dRoot,KdTree::index part,
//        const KdTree& atTree, KdTree::index aRoot, float *bounds) const {
// computes upper and lower bounds on (anisotropic) distance between
// points in two trees
// dRoot : message tree index
// part  : part index (remember, all parts share a common message tree)
// atTree: domain tree
// aRoot : domain tree index
// bounds: storage for the solutions

 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
*/
bool MultiTree::check_inside(const Tr_idx dRoot, const Tr_idx part,
        const KdTree& atTree, const Tr_idx aRoot) const {
    
    //atCenter, *dOffset, *messCenter, *bw;
    
    const float *atCenter   = atTree.center(aRoot);  // domain tree center
    const float *messCenter = center(dRoot);         // message tree center
    const float *dOffset    = offset_all(part);      // offset for part
    for (unsigned int k=0; k<dims; k++){
        const float s1 = dOffset[k] + messCenter[k] - atCenter[k];
        const float s2 = range(dRoot)[k] + atTree.range(aRoot)[k];
        if ((s1>s2)||(s1<-s2))
               return false;
    }
    return true;
}
    
bool MultiTree::maxminDist(const Tr_idx dRoot, const Tr_idx part,
        const KdTree& atTree, const Tr_idx aRoot, float *bounds) const {

    float result1=0, result2=0, result3 = 0;
    const float *atCenter, *dOffset, *messCenter, *bw;
    
    atCenter   = atTree.center(aRoot);  // domain tree center
    messCenter = center(dRoot);         // message tree center
    bw         = bw_all(part);          // bandwidth for part
    dOffset    = offset_all(part);      // offset for part
    
    for (unsigned int k=0; k<dims; k++){
        const float df   = dOffset[k] + messCenter[k] - atCenter[k];
        const float dc   = fastabs(df);
        const float R2   = atTree.range(aRoot)[k];
        const float R1   = range(dRoot)[k];
        const float sr   = R2 + R1;
        const float dr   = R1 - R2;

        // G_{dd} = max( |h-h'| - (\heta + \heta'),0)^2:  best possible score
        const float one_over_bw  = bw[k];
        const float tmp3         = dc - sr;
        if (tmp3>0) result1     -= tmp3*tmp3*one_over_bw;
       
        // G_{uu} = max( |h-h'| + (\heta + \heta'),0)^2:  worst possible score
        const float tmp4 = dc + sr;
        result2         -= tmp4*tmp4*one_over_bw;

        // G_{uu} = max( |h-h'| + (\heta - \heta'),0)^2:  worst at domain, with best from source 
        const float tmp5     = dc + dr;
        if (tmp5>0) result3 -= tmp5*tmp5*one_over_bw;
    }
    bounds[0] = result1/2;
    bounds[1] = result2/2;
    bounds[2] = result3/2;
    return (bounds[0]==bounds[1]);  // boolean indicating that the bounds are tight 
}


float MultiTree::sinDist(const Tr_idx dRoot, const Tr_idx part,
        const KdTree& atTree, const Tr_idx aRoot) const {

    float result1=0;
    const float *atCenter, *dOffset, *messCenter, *bw;
    
    atCenter   = atTree.center(aRoot);  // domain tree center
    messCenter = center(dRoot);         // message tree center
    bw         = bw_all(part);          // bandwidth for part
    dOffset    = offset_all(part);      // offset for part
    
    for (unsigned int k=0; k<dims; k++){   
        const float tmp3         = fastabs(dOffset[k] + messCenter[k] - atCenter[k]) - (atTree.range(aRoot)[k] + range(dRoot)[k]);
        if (tmp3>0) result1     -= tmp3*tmp3*bw[k];
    }
    return result1/2;
 }

void MultiTree::l2Dist(const float* loc, const Tr_idx dRoot, float *bounds) const{
    float result1=0, result2=0;
    const float  *messCenter;
    messCenter = center(dRoot);  //message tree center    
    for (unsigned int k=0; k<dims; k++){
        const float df = fastabs(messCenter[k] - loc[k]);
        const float Ran  = range(dRoot)[k];        
        const float tmp3 = df - Ran;
        if (tmp3>0) result1 += tmp3*tmp3;
        const float tmp4 = df + Ran;
        result2 += tmp4*tmp4;                
    }
    bounds[0] = result1/2;
    bounds[1] = result2/2;
    return;
}


void MultiTree::l2Dist(const float* loc, const Tr_idx dRoot, float *bounds, float scale,float scale_gt,float f_scale, float thr_loss) const{
    float result1=0, result2=0;
    const float  *messCenter;
    messCenter = center(dRoot);  //message tree center    
    for (unsigned int k=0; k<dims; k++){
        const float df = fastabs(messCenter[k] - loc[k]);
        const float Ran  = range(dRoot)[k];        
        const float tmp3 = df - Ran;
        if (tmp3>0) result1 += tmp3*tmp3;
        const float tmp4 = df + Ran;
        result2 += tmp4*tmp4;                
    }
    const float diff_scale = -((float) (scale - scale_gt)*(scale-scale_gt))*f_scale;
    
    // const float diff_scale = ((float) my_pow_2(scale - scale_gt))*f_scale;
    // bounds_loc[1] += diff_scale;
    // bounds_loc[0] += diff_scale;
    //mexPrintf("%.3f \n",thr_loss);
    bounds[0] = my_min(result1 + diff_scale,thr_loss);
    bounds[1] = my_min(result2 + diff_scale,thr_loss);
    return;
}

// Calculate the statistics of level "root" based on the statistics of
//   its left and right children.
void MultiTree::calcStats(MultiTree::index root, bool only_left) {
    
    Tr_idx leftI = left(root), rightI=right(root);// get children indices
    // if the left ball is the same as the right ball  don't count the weight twice    
    float * const root_u = weight_all_u(root);
    float * const left_u = weight_all_u(leftI);
    
    if (only_left)
        memcpy(root_u,left_u,NW*nval*sizeof(float));    
    else {
        float * const root_l = weight_all_l(root);
        float * const root_b = weight_all_b(root);
    
        const float * const left_l = weight_all_l(leftI);
        const float * const left_b = weight_all_b(leftI);

        const float * const right_u = weight_all_u(rightI);
        const float * const right_l = weight_all_l(rightI);
        const float * const right_b = weight_all_b(rightI);

        const bool do_l = NW>2;
        for (Tr_idx val =0; val<nval; val++) {
            const  float  lu = left_u[val], ru = right_u[val];
            root_u[val] = my_max(lu, ru);
            const  float lb= left_b[val], rb = right_b[val];
            root_b[val] = my_min(lb, rb);
            if (do_l)
            {
            const  float ll = left_l[val], rl = right_l[val];
            root_l[val] = my_max(ll, rl);
            }
        }
    }
}


//--------------------------------------------------------------------------------------------------------------------------------------------
// matlab functions
//--------------------------------------------------------------------------------------------------------------------------------------------
// Load the arrays already allocated in matlab from the given structure.
MultiTree::MultiTree(Mat_in structure) : KdTree(structure) {
    bandwidth = (float*) mxGetData(mxGetField(structure, 0, "bws"));
    offset    = (float*) mxGetData(mxGetField(structure, 0, "offsets"));
    weight    = (float*) mxGetData(mxGetField(structure, 0, "weights"));
    nval      = (Tr_idx)  ceil(mxGetScalar(mxGetField(structure, 0, "n")));
    NW        = (Tr_idx)  ceil(mxGetScalar(mxGetField(structure, 0, "nw")));    
    nval3     =  NW*nval;
}

// Create new matlab arrays and put them in the given structure
mxArray* MultiTree::createInMatlab(Mat_in pointsMatrix, Mat_in weightsMatrix, \
        Mat_in offsetsMatrix, Mat_in bwsMatrix, Mat_in dims0, Mat_in dims2,int NW,float fct,float ho,float vo,int Noffsets) {
    
    mxArray* structure;
    float *_points, *_weights, *_offsets, *_bws;
    
    // get fields from input arguments
    unsigned int Nd = mxGetM(pointsMatrix);    
    int Np  = mxGetN(pointsMatrix);
    int nv  = mxGetM(weightsMatrix)/NW;
    
    _points    = (float*)mxGetData(pointsMatrix);
    _weights   = (float*)mxGetData(weightsMatrix);    
    _offsets   = (float*)mxGetData(offsetsMatrix);
    _bws       = (float*)mxGetData(bwsMatrix);
    
    // create structure, populate it, and get handles to the arrays
    structure = mxCreateStructMatrix(1, 1, nfields, FIELD_NAMES);
    
    mxAddField(structure, "n");
    mxAddField(structure, "nw");
    mxAddField(structure, "weights");
    mxAddField(structure, "offsets");
    mxAddField(structure, "bws");
    
    double *pdim;
    pdim     = mxGetPr(dims0);
    int h0   = (int) pdim[0];
    int v0   = (int) pdim[1];
    pdim     = mxGetPr(dims2);
    int h2   = (int) pdim[0];
    int v2   = (int) pdim[1];
    unsigned int Np2 = h2*v2;
    unsigned int NpS = Np + Np2;
    mwSize dimsd[2]  = {1, 2};
    
    mxSetField(structure, 0, "d0",      mxCreateNumericArray(2, dimsd, mxSINGLE_CLASS, mxREAL));
    mxSetField(structure, 0, "d2",      mxCreateNumericArray(2, dimsd, mxSINGLE_CLASS, mxREAL));

    float* d;
    d = (float *) mxGetData(mxGetField(structure, 0, "d0"));
    d[0] = h0;  d[1] = v0;
    d = (float *) mxGetData(mxGetField(structure, 0, "d2"));
    d[0] = h2;  d[1] = v2;
 
    mxSetField(structure, 0, "D",       mxCreateDoubleScalar((double) Nd));
    mxSetField(structure, 0, "N",       mxCreateDoubleScalar((double) Np));
    mxSetField(structure, 0, "n",       mxCreateDoubleScalar((double) nv));
    mxSetField(structure, 0, "nw",      mxCreateDoubleScalar((double) NW));
    mxSetField(structure, 0, "leftch",  mxCreateNumericMatrix(1, NpS, mxUINT32_CLASS, mxREAL));
    mxSetField(structure, 0, "rightch", mxCreateNumericMatrix(1, NpS, mxUINT32_CLASS, mxREAL));

    mwSize dims_centers[2] = {Nd,   NpS};
    mwSize dims_weights[2] = {NW*nv,NpS};
    mwSize dims_offsets[2] = {Nd,   Noffsets};

    mxSetField(structure, 0, "centers", mxCreateNumericArray(2, dims_centers,  mxSINGLE_CLASS, mxREAL));
    mxSetField(structure, 0, "ranges",  mxCreateNumericArray(2, dims_centers,  mxSINGLE_CLASS, mxREAL));
    mxSetField(structure, 0, "weights", mxCreateNumericArray(2, dims_weights, mxSINGLE_CLASS, mxREAL));
    mxSetField(structure, 0, "offsets", mxCreateNumericArray(2, dims_offsets, mxSINGLE_CLASS, mxREAL));
    mxSetField(structure, 0, "bws",     mxCreateNumericArray(2, dims_offsets, mxSINGLE_CLASS, mxREAL));

    // initialize arrays
    float* centers   = (float *) mxGetData(mxGetField(structure, 0, "centers"));
    float* offsets   = (float *) mxGetData(mxGetField(structure, 0, "offsets"));
    float* bws       = (float *) mxGetData(mxGetField(structure, 0, "bws"));
    
    // the leaf nodes for centers/points are filled in (algorithm will deal with parents)
    const Tr_idx NdNp2 = Nd*Np2, NdNp = Nd*Np;
    for (Tr_idx j=0, i=NdNp2; j<NdNp; i++, j++)
        centers[i] = _points[j];
    
    float  *weights = (float *) mxGetData(mxGetField(structure, 0, "weights"));
    memcpy((weights + NW*nv*Np2), _weights, NW*nv*Np*sizeof(float));
    
    
    const Tr_idx NdNv = dims_offsets[0]*dims_offsets[1];
    for (Tr_idx i=0; i<NdNv; i++) {
        offsets[i]  = _offsets[i];
        bws[i]      = _bws[i];
    }
    MultiTree mess(structure);
    mess.buildTree(fct,ho,vo);
    return structure;
}
