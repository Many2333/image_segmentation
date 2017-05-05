#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "mex.h"
#include "defs.h"
#include "MultiTree.h"
#include "Interval.h"
#include "BoundingFunction.h"

// store information about trajectory followed by search algorithm -
// useful for visualizing the bounds in matlab
#define push_to_track \
{\
    Tr->at(0).push_back((float) (parentNode+1));\
    Tr->at(1).push_back(upper);\
    Tr->at(2).push_back((float) (scale+1));\
    Tr->at(3).push_back((float) (mixt+1));\
    Tr->at(4).push_back((float) (leftNode+1));\
    Tr->at(5).push_back((float) (rightNode+1));\
    Tr->at(6).push_back((float) (vl[0]));\
    Tr->at(7).push_back((float) (vl[1]));\
    Tr->at(8).push_back((float) (path));\
}

// common code for cascaded detection and branch and bound
#define treat_new_node \
float upper             = currentState->upper;                  \
const Tr_idx parentNode = currentState->domainNode;             \
const float lower       = currentState->lower;                  \
const int nparts        = currentState->nparts;                 \
const int scale         = currentState->scale;                  \
const int mixt          = currentState->mixt;                   \
const Tr_idx leftNode   = domainTree->left( parentNode);        \
const Tr_idx rightNode  = domainTree->right(parentNode);        \
const bool is_thin      = ((parentNode==leftNode)&&(currentState->isfine));\
bool is_solution;                                               \
if (Bounder.doapp)                                              \
    is_solution = is_thin&&(upper==lower);                      \
else                                                            \
    is_solution = is_thin;                                      \
float vl[2]  = {0,0};                                           \
if ((is_thin)&(!is_solution))                                   \
{                                                               \
    push_to_track                                               \
    Interval *newState  = new Interval(currentState, parentNode);\
    Bounder.refine(newState,currentState,threshold);            \
    upper = newState->upper;                                    \
    if (upper>threshold)                                        \
    {                                                           \
        if (do_bb)                                              \
            pH->push(newState);                                 \
        else                                                    \
        {                                                       \
            configurations->at(0).push_back(parentNode);        \
            for (int p = 0; p<nparts; p++)                      \
                configurations->at(p+1).push_back(newState->Mess_supporters[p].at(0).idx);\
            configurations->at(nparts+1).push_back(scale);      \
            configurations->at(nparts+2).push_back(mixt);       \
            configurations->at(nparts+3).push_back(path);       \
            scores->push_back(upper);                           \
         }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
    newState->cleanup(); delete newState;                       \
    }       if (!do_bb) return;                                 \
}                                                               \
if  (is_solution){                                              \
    configurations->at(0).push_back(parentNode);                \
    for (int p = 0; p<nparts; p++)                              \
        configurations->at(p+1).push_back(currentState->Mess_supporters[p].at(0).idx);\
    configurations->at(nparts+1).push_back(scale);              \
    configurations->at(nparts+2).push_back(mixt);               \
    configurations->at(nparts+3).push_back(path);               \
    scores->push_back(upper);                                   \
    if (Bounder.learn)                                          \
        scores->push_back(currentState->loss);                  \
}


// contains all of the information necessary to compute the bound of an interval's score
static BoundingFunction Bounder;

// Branch & bound (based on Lampert et al BB code)
static void bb(Interval_heap *pH, cnfs *configurations, vec *scores, \
        const KdTree *domainTree, const float threshold, const int nsol, arr *Tr,const int mode){
    // pH:     heap of intervals (interval = domain node + associated source nodes (`supporters')
    // cnfs:   solution(s) found by algorithm
    // scores: scores of the associated solutions
    // domainTree: kdTree with domain nodes
    // threshold:  interval score where we stop search 
    // nsol:       max number of solutions  
    // Tr:         array where tracking information is stored
    // mode:       0: normal (optimization)    -1: just display data (for debugging/visualization of results)
    
    
    int cnt_sol =0;     // number of solutions found so far 
    bool cont = 1;
    
    while (cont){
        Interval* currentState = pH->top(); 
        pH->pop();
        if (currentState->upper<threshold) {
            currentState->cleanup();
            delete currentState;
            break;
        }
        
        const int path   = 0;         // ignore - this is just necessary for push_to_track
        const bool do_bb = 1;         // ignore - used to make the `treat_new_node' code work
        treat_new_node

        if (is_solution)
        {
            push_to_track
            cont = (++cnt_sol)<nsol;
        }
        if (!is_thin)
        {
            //--------------------------------------------------------------
            // keep going: branch - bound - traverse
            //--------------------------------------------------------------
            // number of children of current domain node  
            // when we reach a leaf, leftNode points back to itself, rightNode is valued NO_CHILD
            // or else, when rightNode (would point to) a rectangle that 
            // contains no data, it is valued NO_CHILD again  
            const int nch = 1 + (rightNode!=NO_CHILD);        
            currentState->sortSupporters();             // accelerates pruning 

            for (int child=0; child<nch; child++) {
                const Tr_idx childNode  = (child==0)?leftNode:rightNode;
                Interval *newState  = new Interval(currentState, childNode);
                const int out = Bounder.upper_bound(newState, currentState,threshold,mode);
                vl[child]  = newState->upper;
                if ((newState->upper>threshold)&&(out))
                    pH->push(newState);
                else
                { newState->cleanup(); delete newState;}
            }
            push_to_track
        }
        currentState->cleanup();
        delete currentState;
        
		if (pH->empty())
			cont = 0;
    }
    // cleanup & return
    while (!pH->empty())
    { Interval *fs  = pH->top(); fs->cleanup();  delete fs; pH->pop(); }
    return;
}

// cascade code (based on Lampert's detection cascade code)
static void tree(Interval* currentState, cnfs *configurations, vec *scores, \
        const KdTree *domainTree, const float threshold, arr *Tr, const int path,const int mode) {
    // input arguments: as for bb
    //--------------------------------------------------------------
    //  if we have a new solution, store it and return
    //--------------------------------------------------------------
    const bool do_bb = 0;
    Interval_heap *pH;          // ignore, used just to make treat_new_node work
    
    treat_new_node

    if (is_solution) { push_to_track; return;} 
    //--------------------------------------------------------------
    // keep going: split - bound - traverse
    //--------------------------------------------------------------
    int ch_start, nch; 
   
     // some additional complexity - th code below allows us to report supporters
    // if we have mode =-1:
    // in that case the `path' variables provides the  encoding of the left/right 
    // directions followed by a (previous) search algorithm until a desired solution
    // was found. Instead of the default mode, which explores both left & right
    // children, we now chose the single child dictated by the `path' variable
    // and report all the associated information about it within the `supporters.m' file. 
    if (mode<0)
    {
        // path contains info in form: 3^N (direction_0+1) + 3^(N-1) + (direction_1 + 1) + .. 3^(N-k)*(direction_k + 1)
        int direction = path % 3;       // get `direction_level'
        FILE *fp  = fopen("supporters.m","a");
        fprintf( fp,"node(%i) = %i; \n",currentState->level+1,parentNode);
        fclose(fp);
        ch_start    = direction-1;      // constrain the options of the search algorithm
        nch         = direction; 
    }
    else
    {   // as in bb code
        ch_start = 0;
        nch = 1 + (rightNode!=NO_CHILD); 
    }
    
    // same code as in bb - but now instead of pushing the split nodes into 
    // the priority we recursively explore their trees 
    int path_sent_out; 
    for (int child=ch_start;child<nch;child++) {
        const Tr_idx childNode  = (child==0)?leftNode:rightNode;
        Interval *newState  = new Interval(currentState, childNode);
        Bounder.upper_bound(newState, currentState,threshold,mode);
        vl[child] = newState->upper;
        if (newState->upper>threshold) {
            newState->sortSupporters();
            path_sent_out = (mode<0)?(path/3):(3*path + child+1);
            tree(newState, configurations, scores, domainTree, threshold, Tr,path_sent_out,mode);
        }
        newState->cleanup();
        delete newState;
    }
	push_to_track
}

void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    //------------------------------------------------------------------
    // store incoming Root/Part messages into two separate vectors
    // vector index indicates pyramid scale
    //------------------------------------------------------------------
    int cnt = 0;
    const int TROOT_IND  = cnt++;
    const int TPART_IND  = cnt++;
    const int NPART_IND  = cnt++;
    const int DOMAIN_IND = cnt++;
    const int THRESH_IND = cnt++;
    const int NBEST_IND  = cnt++;
    const int MODE_IND   = cnt;
    
    
    //   [out,v,tr]  = mex_traverse(root_trees,part_trees,nparts,domain_kdtree,thr,nbest,1,feat,padx+1,pady+1,w_root,w_part,bs);
    // inputs for bounding-based unary potentials
    cnt = MODE_IND + 1;
    const int PYRA_IND   = cnt++;
    const int PADX_IND   = cnt++;
    const int PADY_IND   = cnt++;
    const int WROOT_IND  = cnt++;
    const int WPART_IND  = cnt++;
    const int BS_IND     = cnt++;

    // inputs for loss-augmented inference
    cnt = MODE_IND + 1;
    const int GTR_IND       = cnt++;
    const int SCALE_IND     = cnt++;
    const int F_SCALE_IND   = cnt++;
    const int LAMBDA_IND    = cnt++;     
    const int THR_LOSS_IND  = cnt++;
    const int BOOL_IND      = cnt++;
    
    // inputs for supporter generation
    cnt = MODE_IND + 1;
    const int PATH_IND      = cnt++;
    const int SC_IND        = cnt++;
    const int MX_IND        = cnt++;
  
    int nscales  = mxGetNumberOfElements(prhs[TROOT_IND]);
    // number of mixtures
    int nmixt    = (int) mxGetScalar(mxGetField(mxGetCell(prhs[TROOT_IND],0),0,"n"));
    int npartf   = (int) mxGetScalar(mxGetField(mxGetCell(prhs[TPART_IND],0),0,"n"));
    
    // load the source trees for the root/part filters 
    Mstree MtreeRoot;
    for (int s=0;s<nscales;s++)
        MtreeRoot.push_back(mxGetCell(prhs[TROOT_IND],s));
    
    Mstree MtreePart;
    int nparts = 1 + (int) mxGetScalar(prhs[NPART_IND]);
    if (nparts>1)
        for (int s=0;s<nscales;s++)
            MtreePart.push_back(mxGetCell(prhs[TPART_IND],s));
    
    // load the domain tree 
    KdTree domainTree   = KdTree(prhs[DOMAIN_IND]);

    float threshold     = (float) mxGetScalar(prhs[THRESH_IND]);
    
    // number of wanted solutions (if negative, means we are not doing BB)
    const int  nwt      = (nrhs>NBEST_IND)?((int) mxGetScalar(prhs[NBEST_IND])):0;
	const bool do_bb    = nwt>0;
    
    // are we optimizing (mode=0), or reporting results? (mode=-1)
    const int mode      = (nrhs>MODE_IND)?mxGetScalar(prhs[MODE_IND]):0;
    int sc_wt = 0;
    int mx_wt = 0;
    int path  = 0;
    if (mode<1)
    {
        Bounder.setup(&domainTree,&MtreeRoot,&MtreePart); // standard/show supporters
        if (mode<0)
        {
            fclose(fopen("supporters.m","w"));
            path  = (int) mxGetScalar(prhs[PATH_IND]);
            sc_wt = (int) mxGetScalar(prhs[SC_IND]);
            mx_wt = (int) mxGetScalar(prhs[MX_IND]);
        }
    }
    if (mode==1) // unary bounds
        Bounder.setup(&domainTree,&MtreeRoot,&MtreePart,nmixt,npartf,nscales,\
                (int) mxGetScalar(prhs[PADX_IND]),(int) mxGetScalar(prhs[PADY_IND]),\
                prhs[PYRA_IND],prhs[WROOT_IND],prhs[WPART_IND],prhs[BS_IND]);

    if (mode==2) // loss augmented inference 
        Bounder.setup(&domainTree,&MtreeRoot,&MtreePart,prhs[GTR_IND],mxGetScalar(prhs[SCALE_IND])-1,\
                (float) mxGetScalar(prhs[F_SCALE_IND]),nscales,nparts,mxGetScalar(prhs[LAMBDA_IND]),\
                mxGetScalar(prhs[THR_LOSS_IND]),mxGetScalar(prhs[BOOL_IND]));
    
    // array with the tracking information 
    int  nTrack = 9; arr Tr;  Tr.resize(nTrack);
    
    // heap data structure for prioritized search 
    Interval_heap Hp;
    cnfs configurations;
    configurations.resize(nparts+4);
    vec  scores;
    
    for (int sc = 0; sc<nscales;sc++)
    {
        Tr_idx start_node = domainTree.root();
        
        // get size of image at the current scale
        // and make sure the domain node covers all of it

        // size of image
        float max_h = MtreeRoot[sc].center(0)[0] + MtreeRoot[sc].range(0)[0];
        float max_v = MtreeRoot[sc].center(0)[1] + MtreeRoot[sc].range(0)[1];
        bool cont = 1;
        
        while (cont)
        {
            // check for domain node's extent 
            Tr_idx child_lu = domainTree.left(start_node);
            if ((max_h<((domainTree.center(child_lu)[0] + domainTree.range(child_lu)[0])))&&\
                (max_v<((domainTree.center(child_lu)[1] + domainTree.range(child_lu)[1]))))
                start_node = child_lu;
            else
                cont = 0;
        }
		for (int mx=0; mx<nmixt; mx++)
        {
            Interval* fullspace = new Interval(start_node,sc,mx,nparts);
            if (do_bb)		// push fullspace into priority queue
                Hp.push(fullspace);
            else            // traverse fullspace tree on-the-fly
            {
                if (mode>=0)
                    tree(fullspace, &configurations, &scores, &domainTree, threshold, &Tr,path,mode);
                else
                    if ((sc==sc_wt)&&(mx==mx_wt))
                        tree(fullspace, &configurations, &scores, &domainTree, threshold, &Tr,path,mode);
                
                fullspace->cleanup();   delete fullspace;
            }
       }
   }
    if (do_bb)
        bb(&Hp, &configurations, &scores, &domainTree, threshold,nwt, &Tr,mode);
    
    Bounder.cleanup();
    
    
    int nSol = scores.size()/(1 + (mode==2));
    
    // send data back to matlab
    plhs[0]  = mxCreateDoubleMatrix(nSol,        nparts+4, mxREAL);
    plhs[1]  = mxCreateDoubleMatrix(scores.size(),1, mxREAL);
    	
    double *solution = mxGetPr(plhs[0]);
    double *score    = mxGetPr(plhs[1]);
    for (int j=0;j<(nparts+4);j++)
        for (int i=0;i<nSol;i++)
        *(solution++) = (double) (configurations[j][i]+1);
    		
    for (unsigned int i=0;i<scores.size();i++)
        *(score++) = (double) scores[i];
    
    int nGrid   = (int) Tr[0].size();
    plhs[2] = mxCreateDoubleMatrix(nTrack, nGrid, mxREAL);
    double *track    = mxGetPr(plhs[2]);
    for (int gr= 0;gr<nGrid;gr++)
        for (int d=0;d<nTrack;d++)
            *(track++) = (double) Tr[d][gr];
    
    solution = NULL; score = NULL;
    scores.erase(scores.begin(),scores.end());

    for (int j=0;j<(nparts+4);j++)
        configurations[j].erase(configurations[j].begin(),configurations[j].end());
    configurations.erase(configurations.begin(),configurations.end());
}

 