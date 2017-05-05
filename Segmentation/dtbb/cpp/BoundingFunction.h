#ifndef _boundingfunction
#define _boundingfunction

#include <vector>
#include <iostream>
#include <algorithm>
#include "Interval.h"
#include "MultiTree.h"

#define min_val (float) -1e20;
#define my_max(a, b) ((a)>(b)? (a):(b))
#define my_pow_2(a) ((a)*(a))

// used for Bounding unary scores (just initializatoin)

struct lookup {
    int ncells;
    int *i_part;
    int *i_offset;
    lookup()
    { ncells = 0; };
    
    void set_to(mxArray *Mpi) 
    {
        double *pi =  mxGetPr(Mpi);
        ncells = mxGetN(Mpi);
        i_part   = new int[ncells];
        i_offset = new int[ncells];
        for (int i =0;i<ncells;i++)
        {
            i_part[i]   = (int) *(pi++);
            i_offset[i] = (int) *(pi++);
        }
    }
   void cleanup()
   {
     if (ncells>0)
        {
        delete []i_part;
        delete []i_offset;
        }
   }
    ~lookup()
    {}
};
//typedef  lookup_t;
typedef std::vector<struct lookup>  Lookup;


#ifndef RELEASE
typedef struct {const float *hogs; arr3 weights;    int dims[3]; int offsets[20][20]; int padh; int padv;} HOG_struct;
typedef std::vector<HOG_struct> HOG_cell;

inline void copy_HOG(mxArray *inm, HOG_struct *in,int padh,int padv,bool copy)
{
    in->hogs = (float*) mxGetPr(inm);
    int *ds = (int *)  mxGetDimensions(inm);
    in->dims[0] = ds[0]; 
    in->dims[1] = ds[1]; 
    in->dims[2] = ds[2];
    in->padh    = padh;
    in->padv    = padv;
    if (copy)
    {
        int cnt = 0;
        in->weights.resize(ds[1]);
        for (int d1=0;d1<ds[1];d1++)
        {
            in->weights[d1].resize(ds[2]);
            for (int d2=0;d2<ds[2];d2++)
              in->weights[d1][d2].resize(ds[0]);
        }
        // image will be 50 x 100    (V X H)
        // we will have in->weights[50][100][32]   (V X H X NHOG)
        for (int d2=0;d2<ds[2];d2++)
            for (int d1=0;d1<ds[1];d1++)
                for (int d0=0;d0<ds[0];d0++)
                    in->weights[d1][d2][d0] = in->hogs[cnt++];
    }
    else
    {
        // horizontal and vertical offsets with respect to central pixel ->
        // by what amount should we move the pointer to HOGs 
        for (int h = 0; h<20;h++)
            for (int v = 0; v<20;v++)
                in->offsets[v][h] = ds[0]*v + ds[0]*ds[1]*h;
    }
};

inline float inprod_HOG(HOG_struct *filter, HOG_struct *image, int h0, int v0)
{
    int sv = filter->dims[1];
    int sh = filter->dims[2];
    
    int i0 = image->dims[0];
    int i1 = image->dims[1];
    const float *starting_point = image->hogs + i0*(v0+ image->padv ) + i0*i1*(h0 + image->padh);

    float s2 = 0;
    for (int v = 0; v<sv;v++)
    {
        float s1 = 0;
        for (int h = 0; h<sh;h++)
        {
            const float *im = starting_point + image->offsets[v][h];
            const vec    wv = filter->weights[v][h];
            s1 +=    wv[0]*im[0]+wv[1]*im[1]+wv[2]*im[2]+wv[3]*im[3]+wv[4]*im[4]+wv[5]*im[5]+wv[6]*im[6]+wv[7]*im[7]+wv[8]*im[8]\
                    +wv[9]*im[9]+wv[10]*im[10]+wv[11]*im[11]+wv[12]*im[12]+wv[13]*im[13]+wv[14]*im[14]+wv[15]*im[15]\
                    +wv[16]*im[16]+wv[17]*im[17]+wv[18]*im[18]+wv[19]*im[19]+wv[20]*im[20]+wv[21]*im[21]+wv[22]*im[22]\
                    +wv[23]*im[23]+wv[24]*im[24]+wv[25]*im[25]+wv[26]*im[26]+wv[27]*im[27]+wv[28]*im[28]+wv[29]*im[29]\
                    +wv[30]*im[30]+wv[31]*im[31];
        }
        s2 += s1;
    }
    return s2;
}

inline int index(const int level,const int partind,const int h,const int v, const int *cumpix,const int *nscore,const int *sv)
{
    return cumpix[level]  + nscore[level]*partind + h*sv[level] + v;
}

#endif

class BoundingFunction{
public:
    KdTree *domainTree;     // `Domain' Kd Tree
    Mstree *mRoot, *mPart;  // Multi-Trees for Root and Part terms
    bool use_two;
    bool learn ;
    bool maximize;
    double lambda;
    arr3 pos;
    float scale_gt;
    float f_scale;
    float *allscores;
    float thr_loss;
    float *bs;
    bool doapp;
    int *nscore, *cumpix, *svert;
    int hasparts;
	#ifndef RELEASE
    Lookup   Lookup_info;
    HOG_cell Pyra;
    HOG_cell Filters_root;
    HOG_cell Filters_part;
    
    void refine(Interval* curstate, const  Interval* parstate,const float thr){
        const int scale   = curstate->scale;
        const int mixt    = curstate->mixt;
        const int nParts  = curstate->nparts;
        float anticipated = parstate->upper; 

        const Tr_idx domainNode = curstate->domainNode;
        float tot_score=0;
        for (int part = 0; part<nParts;part++){
            const bool is_first       = (part==0);
            const MultiTree *messTree = (is_first)? (&(mRoot->at(scale))):(&(mPart->at(scale)));
            const Tr_idx     mixt_ind = (is_first)? ((Tr_idx) mixt)      :((Tr_idx) ((mixt)*(nParts-1) + part-1));
            
            //  `part' source nodes supporting the parent of `curstate' domain
            const supporterVec *supporters  = &(parstate->Mess_supporters[part]);
            const int nsupporters           = (int) supporters->size();
            
            const int   pidx    = (mixt)*(nParts) + part;
            const int   sc      = (is_first)? 1:2;                   // account for different scale 
            const float offset  = (is_first)? bs[mixt_ind]:0;        // svm's beta

            float best_so_far  = -10;   
            int idx_best = -1;
            
            float max_v = -10;
            for (int k=0; k<nsupporters;k++)
            {
                const float this_val = supporters->at(k).val;
                if (this_val>best_so_far)
                {
                    const int     idx = supporters->at(k).idx;
                    const float *cens = messTree->center(idx);
                    const int h = (int) (cens[0]*sc);
                    const int v = (int) (cens[1]*sc);
                    
                    const int idxhv = index(scale, pidx, h,  v, cumpix,nscore,svert);
                    float score = allscores[idxhv];
                    if (score==0)
                    {
                        if (is_first)
                            score = inprod_HOG(&Filters_root[mixt_ind],&Pyra[scale+10],    h-1,  v-1);
                        else
                            score = inprod_HOG(&Filters_part[mixt_ind],&Pyra[scale],       h-1,  v-1);
						score = score + offset;
                        allscores[idxhv] = score;
                    }
                    const float score_tot = score +  messTree->sinDist(idx,mixt_ind, *domainTree, domainNode);
  
                    if (score_tot>best_so_far)
                    {
                        best_so_far = score_tot;
                        idx_best    = idx;
                    }
                }
                max_v = my_max(max_v,this_val);
            }
            anticipated += (best_so_far - max_v);
            if (anticipated<thr)
            {
                curstate->upper = thr - 1;
                return;
            }
            supportS sup;
            sup.idx = idx_best;
            sup.val = best_so_far;
            curstate->Mess_supporters[part].push_back(sup);
            tot_score = tot_score + best_so_far;
        }
        
        curstate->upper = tot_score;
        curstate->lower = tot_score;
        curstate->loss  = 0;
    };
    #endif
            
    
    int upper_bound_part(Interval* curstate, const  Interval* parstate,const float thr,const int mode,const int part_out=-1){
        float largest_upper_bound =0;
        
        bool isfine = 1;
        const int scale = curstate->scale;
        const int nParts = curstate->nparts;

        // accumulate upper bounds from all parts
        const Tr_idx domainNode  = curstate->domainNode;
        const bool do_lrn       = learn;
        const float min_val_u   = thr + min_val;
        
  //      counter++;
        float *ls = new float[nParts];
        for (int part = 0; part<nParts;part++){
            
            Tr_idx mixt_ind;
            Tr_idx offs_ind;
            const MultiTree *messTree;
            bool hrd;
            if (part_out>=0)
            {
                mixt_ind  = Lookup_info.at(part_out).i_part[part];
                offs_ind  = Lookup_info.at(part_out).i_offset[part];
                messTree  = (&(mRoot->at(scale)));
                hrd      = 1;
            }
            else
            {
                const bool is_first       = (part==0);
                messTree = (is_first)? (&(mRoot->at(scale))):(&(mPart->at(scale)));
                hrd    = 0;
            }
            
           //  mexPrintf("filter %i \t cell %i \t  off %i \t partlet %i \n",part_out,part,offs_ind,mixt_ind);
            //  `part' source nodes supporting the parent of `curstate' domain
            const supporterVec *supporters  = &(parstate->Mess_supporters[part]);
            const int nsupporters           = (int) supporters->size();
            
            // each supporter will have nchildren children, allocate space for
            // the upper bound of their support
            const int nchildren     = 2;
            const int npr           = nchildren*nsupporters;
            int* indexes     = new int[npr];
            
            float lumess = min_val_u;
            int cn = 0;
            for (int k=0; k<nsupporters; k++){
                // condition:  upper bound of  supporter's contribution
                // should be larger than the the (highest) lower bound found so far
                // get k-th supporter of parent-domain-node
                const Tr_idx kthSup = supporters->at(k).idx;
                for (int i=0;i<nchildren;i++) // loop over children of k-th supporter
                {
                    Tr_idx  messChild;
                    messChild = (i==0)? messTree->left(kthSup):messTree->right(kthSup);
                    if (messChild==NO_CHILD) continue;

                    const bool overlaps  = messTree->check_inside(messChild,offs_ind, *domainTree, domainNode);
                    if (!overlaps) continue; 
                    
                    // maximal score, as per upper bound
                    lumess = my_max(lumess,messTree->weight_at_u(messChild, mixt_ind));
                    indexes[cn++] = messChild;
                }
            }
            // allocate space for them
            curstate->Mess_supporters[part].resize(cn);
            
            // store them
            for (int k=0; k<cn; k++) {
               supportS sup;
               sup.idx = indexes[k];
               sup.val = 1;
               curstate->Mess_supporters[part][k] = sup;
            }
            delete []indexes;
            
            largest_upper_bound += lumess;
        }
        /*mexPrintf("====================\n");
        mexPrintf(" SU %.3f \t SL %.3f \n",largest_upper_bound,largest_lower_bound);
        mexPrintf("====================\n");
        */
        float loss = 0;
        if (do_lrn)
        for (int part =0;part<nParts;part++)
        {
            loss += ls[part];
        }
        delete []ls;
        
        if (largest_upper_bound==0) {
            curstate->upper = min_val;
            curstate->lower = min_val;
            return 0;
        }
        curstate->upper = largest_upper_bound;
        curstate->lower = -100;
        curstate->loss  = loss;		// ignore
        curstate->isfine = isfine;	// ignore
        if (mode<0) curstate->spitSupporters();
        return  1;
    };
    
    int upper_bound(Interval* curstate, const  Interval* parstate,const float thr,const int mode,const int part_out=-1){
        float largest_upper_bound =0;
        float largest_lower_bound =0;
        
        bool isfine = 1;
        const bool u2   = use_two;
        const int scale = curstate->scale;
        const int mixt  = curstate->mixt;
        const int nParts = curstate->nparts;

        // accumulate upper bounds from all parts
        const Tr_idx domainNode  = curstate->domainNode;
        const bool do_lrn       = learn;
        const float min_val_u   = thr + min_val;
        
        float *ls = new float[nParts];
        for (int part = 0; part<nParts;part++){
          const bool is_first       = (part==0);
            const MultiTree *messTree = (is_first)? (&(mRoot->at(scale))):(&(mPart->at(scale)));
            const Tr_idx     mixt_ind = (is_first)? ((Tr_idx) mixt)      :((Tr_idx) ((mixt)*(nParts-1) + part-1));
  
            bool hard = 0;
  /*          
            Tr_idx mixt_ind;
            Tr_idx offs_ind;
            const MultiTree *messTree;
            bool hrd;
            if (part_out>=0)
            {
                mixt_ind  = Lookup_info.at(part_out).i_part[part];
                offs_ind  = Lookup_info.at(part_out).i_offset[part];
                messTree  = (&(mRoot->at(scale)));
                hrd      = 1;
            }
            else
            {
                const bool is_first       = (part==0);
                messTree = (is_first)? (&(mRoot->at(scale))):(&(mPart->at(scale)));
                Tr_idx     mixt_ind = (is_first)? ((Tr_idx) mixt)      :((Tr_idx) ((mixt)*(nParts-1) + part-1));
                hrd    = 0;
            }
            const bool hard =hrd; 
*/    
        //  `part' source nodes supporting the parent of `curstate' domain
            const supporterVec *supporters  = &(parstate->Mess_supporters[part]);
            const int nsupporters           = (int) supporters->size();
            
            // each supporter will have nchildren children, allocate space for
            // the upper bound of their support
            const int nchildren     = 2;
            const int npr           = nchildren*nsupporters;
            float* ub     = new float[npr];
            for (int i =0;i<npr;i++)
                ub[i] = min_val;
            
            float lumess = min_val_u;
            float llmess = min_val_u;
            
            for (int k=0; k<nsupporters; k++){
                // condition:  upper bound of  supporter's contribution
                // should be larger than the the (highest) lower bound found so far
                if ((hard)||(supporters->at(k).val>=llmess)) {
                    // get k-th supporter of parent-domain-node
                    const Tr_idx kthSup = supporters->at(k).idx;
                    for (int i=0;i<nchildren;i++) // loop over children of k-th supporter
                    {
                        Tr_idx  messChild;
                        messChild = (i==0)? messTree->left(kthSup):messTree->right(kthSup);
                        if (messChild==NO_CHILD) continue;
                        
                        // maximal score, as per upper bound
                        const float Weight_u = messTree->weight_at_u(messChild, mixt_ind);
                        
                        
                        // maximal score, as per lower bound
                        const float Weight_l = u2? Weight_u:messTree->weight_at_l(messChild, mixt_ind);
                        
                        // minimal score, as per lower bound
                        const float Weight_b = messTree->weight_at_b(messChild, mixt_ind);
                        
                        float crit;
                      crit =  Weight_u;
                        
                        // support can only be smaller than that (geometric term is negative)
                        // so, check here if we can skip computation of geometric bound
                        
                        //float bounds_loc[2];
                        if (crit>=llmess) {
                            float bounds[3];    // upper & lower geometric bounds
                            // compute upper and lower bounds of geometric term
                            const bool fin = messTree->maxminDist(messChild,mixt_ind, *domainTree, domainNode, bounds);
                            isfine = (isfine&&(fin));
                            float upper_bound = Weight_u + bounds[0];
                            //float lower_bound = my_max(Weight_l + bounds[1], Weight_b + bounds[2]);
                            float lower_bound = Weight_l + bounds[1];
                            //float lower_bound = Weight_b + bounds[2];
                            
                            if (upper_bound>llmess) {
       						// update upper bound values, as well as largest-lower-bound value
 							    if (lower_bound>llmess)
                                    llmess = lower_bound;
                                if (upper_bound>lumess)
                                    lumess = upper_bound;
                                
                                ub[nchildren*k + i] = upper_bound;
                            }
                        }
                    }
                }
            }
            
           /*  mexPrintf("U   %.3f \t   L %.3f \n",lumess,llmess);*/
            //--------------------------------------------------------
            // loop over all possible supporters, and pick the ones whose
            // upper bound to the contribution exceeds the largest-lower-bound
            //--------------------------------------------------------
            // count how many supporters we have
            int cnt_sup = 0;
            for (int k=0; k<npr; k++)
                if (ub[k]>=llmess)
                    cnt_sup++;
            
            // allocate space for them
            curstate->Mess_supporters[part].resize(cnt_sup);
            
            // store them
            cnt_sup = 0;
            int cn = 0;
            for (int k=0; k<nsupporters; k++) {
                for (int i=0;i<nchildren;i++) // loop over left and right children
                {
                    if (ub[cn]>=llmess) {
                        supportS sup;
                        const Tr_idx kthSup = supporters->at(k).idx;
                        sup.idx = (i==0)?messTree->left(kthSup):messTree->right(kthSup);
                        sup.val = ub[cn];
                        curstate->Mess_supporters[part][cnt_sup] = sup;
                        cnt_sup++;
                    }
                    cn++;
                }
            }
            delete []ub;
            
            largest_upper_bound += lumess;
            largest_lower_bound += llmess;
        }
        /*mexPrintf("====================\n");
        mexPrintf(" SU %.3f \t SL %.3f \n",largest_upper_bound,largest_lower_bound);
        mexPrintf("====================\n");
        */
        float loss = 0;
        if (do_lrn)
        for (int part =0;part<nParts;part++)
        {
            loss += ls[part];
        }
        delete []ls;
        
        if (largest_upper_bound==0) {
            curstate->upper = min_val;
            curstate->lower = min_val;
            return 0;
        }
        curstate->upper = largest_upper_bound;
        curstate->lower = largest_lower_bound;
        curstate->loss  = loss;		// ignore
        curstate->isfine = isfine;	// ignore
        if (mode<0) curstate->spitSupporters();
        return  1;
    };
    

    // load data
    void setup(KdTree *dtree, Mstree *root, Mstree *part) {
        domainTree = dtree;
        mRoot   = root;
        mPart   = part;
        use_two = (bool) (root[0][0].NW ==2);
        learn    = 0;
        maximize = 1;
        doapp    = 0;
        return;
    };
        
    
    
	#ifndef RELEASE
    // unary terms only   
    // load data
    void setup(KdTree *dtree, Mstree *root, const mxArray *ptr) {
        domainTree = dtree;
        mRoot   = root;
        //Lookup_info = lookup_info_;
        use_two = (bool) (root[0][0].NW ==2);
        learn    = 0;
        maximize = 1;
        doapp    = 0;
        hasparts = -1;
       
        int nparts = mxGetNumberOfElements(ptr);
        Lookup_info.resize(nparts);
        for (int p=0;p<nparts;p++)
            Lookup_info[p].set_to(mxGetCell(ptr,p));
                   
        return;
    };

	// loss-augmented cost 
 	void setup(KdTree *dtree, Mstree *root, Mstree *part,const mxArray* ptr,float scale_gt_,\
            float f_scale_,int nscales,int nparts,double lambda_,double thr_loss_,bool do_max) {         
        pos.resize(nscales);
        for (int sc = 0; sc<nscales;sc++)
        {
            double *dt = mxGetPr(mxGetCell(ptr,sc));
            pos[sc].resize(nparts);
            for (int pt = 0;pt<nparts;pt++)
            {
                pos[sc][pt].resize(2);
                pos[sc][pt][0] = *(dt++);
                pos[sc][pt][1] = *(dt++);
            }
        }
        scale_gt   = (int) scale_gt_;
        f_scale    = f_scale_;
        domainTree = dtree;
        mRoot    = root;
        mPart    = part;
        use_two  = (bool) (root[0][0].NW ==2);
        lambda   = lambda_;
        thr_loss = (float) thr_loss_;
        maximize = do_max;
        learn    = 1;
		doapp    = 0;
        return;
    };
    // load data
    void setup(KdTree *dtree, Mstree *root, Mstree *part,int nmixt,int npartf,\
            int nscales,int padh,int padv,const mxArray *pyramid,const mxArray *root_fil,const mxArray *part_fil,const mxArray *bs_fil) {
        domainTree = dtree;
        mRoot   = root;
        mPart   = part;
        use_two = (bool) (root[0][0].NW ==2);
        learn    = 0;
        maximize = 1;
        doapp    = 1;
        //
        nscore = new int[nscales+10];     // npixs per level
        cumpix = new int[nscales+10];     // cumulative sum
        svert  = new int[nscales+10];     // vertical size per level
        bs     = new float[nmixt];
        
        int cum = 0;
        int nparts_tot = nmixt + npartf;            // total number of parts
		
        for (int s=0;s<(nscales+10);s++)
        {
            HOG_struct Level;
            copy_HOG(mxGetCell(pyramid,s),&Level,padh,padv,0);
            Pyra.push_back(Level);
            int npixels = Level.dims[1]*Level.dims[2];
            nscore[s] = npixels;
            cumpix[s] = cum*nparts_tot;
            svert[s]  = Level.dims[1];
            cum      += npixels;
        }
        for (int m=0;m<nmixt;m++)
        {
            HOG_struct Filter;
            copy_HOG(mxGetCell(root_fil,m),&Filter,padh,padv,1);
            Filters_root.push_back(Filter);
            
            float *pr = (float *) mxGetPr(bs_fil);
            bs[m] = pr[m];
        }
        for (int m=0;m<npartf;m++)
        {
            HOG_struct Filter;
            copy_HOG(mxGetCell(part_fil,m),&Filter,padh,padv,1);
            Filters_part.push_back(Filter);
        }
        int nbytes =sizeof(float)*cum*(nparts_tot);
        allscores  = (float*) malloc(nbytes);
        memset(allscores,0,nbytes);
    };
    
    void cleanup(){
        domainTree    = NULL;
        mRoot->erase(mRoot->begin(), mRoot->end());mRoot->clear();
        if (hasparts==-1)
        {
            Lookup_info.erase(Lookup_info.begin(), Lookup_info.end());Lookup_info.clear();
        }
        else
        {
            mPart->erase(mPart->begin(), mPart->end());mPart->clear();
        }
        if (doapp)
        {
            Filters_part.erase(Filters_part.begin(),Filters_part.end()); Filters_part.clear();
            Filters_root.erase(Filters_root.begin(),Filters_root.end()); Filters_root.clear();
            Pyra.erase(Pyra.begin(),Pyra.end()); Pyra.clear();
            delete []nscore;
            delete []cumpix;
            delete []svert;
			delete []bs;
            free(allscores);
        }
        return;
    };
#else
 void cleanup(){
        domainTree    = NULL;
        mPart->erase(mPart->begin(), mPart->end());mPart->clear();
        mRoot->erase(mRoot->begin(), mRoot->end());mRoot->clear();     
        return;
    };
#endif
};

#endif