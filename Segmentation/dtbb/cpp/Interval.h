/*
 Interval.h
 defines interval related data structures
 useful for (1) Bounding and (2) Prioritized search functions
 Copyright (C) 2012 Iasonas Kokkinos
 available under the terms of the GNU GPLv2
 */

#ifndef _Interval
#define _Interval

#ifndef WIN
#include <algorithm>
#endif

#include <queue>
#include "MultiTree.h"
 
typedef struct {Tr_idx idx; float val;} supportS;  // supporter structure: index to supported node, upper bound of support
typedef std::vector<supportS>       supporterVec;   // supporters
typedef std::vector<supporterVec>   supporterScale;

struct support_comparison{
    bool operator()(const supportS &a,const supportS &b) const{
        return a.val>b.val;
    }
};

class Interval{
public:
    float upper;
    float lower;
    float loss;
    int level;
    int scale;
    int nparts;
    int mixt;
    bool isfine;
    
    Tr_idx domainNode,parent;
    supporterScale Mess_supporters;
    bool less(const Interval* other) const {
        return upper < other->upper;
    }
    
    void cleanup()
    {
        int nparts =Mess_supporters.size();
        for (int part=0;part<nparts;part++)
        {
            Mess_supporters[part].erase(Mess_supporters[part].begin(), Mess_supporters[part].end());
        }
        Mess_supporters.clear();
    }
    
    Interval(const Interval* parn,Tr_idx grd_) {
        parent      = parn->domainNode;
        domainNode  = grd_;
        level       = parn->level+1;
        scale       = parn->scale;
        mixt        = parn->mixt;
        nparts      = parn->nparts;
        isfine      = parn->isfine;
        
        Mess_supporters.resize(nparts);
    }
    
    // constructor for whole domain interval (beginning of BB)
    Interval(const Tr_idx root_grid, const int scale_, const int mixt_, const int nparts_){
        domainNode   = root_grid;
        scale  = scale_;
        nparts = nparts_;
        mixt   = mixt_;
        level  = 0;
        isfine = 0;
        // dummy supporter
        supportS supS;
        
        supS.idx = 0;
        supS.val = 0;
        Mess_supporters.resize(nparts);
        for (int pt=0;pt<nparts;pt++) {
            Mess_supporters[pt].push_back(supS);
        }
        parent      = NULL;
        upper       = (float) 1e10;
        lower       = (float)-1e10;
    }
    
    // sorts supporters of interval, so that the ones having larger upper
    // bounds are examined first
    void sortSupporters() {
        for (int pt = 0; pt<nparts; pt++)
            sort(Mess_supporters[pt].begin(), Mess_supporters[pt].end(), support_comparison());
    }
    
    void spitSupporters(){
        FILE *fp = fopen("supporters.m","a");
        for (int pt = 0; pt<nparts; pt++)
        {
            fprintf(fp,"sups{%i}{%i} =  ",level,pt+1);
            
            const supporterVec  *supporters= &Mess_supporters[pt];
            
            int nsupporters = supporters->size();
            if (nsupporters==0) {
                fprintf(fp,"[]; \n");
                return;
            }
            fprintf(fp,"[[");
            // write indexes + upper bounds of expected contribution
            for (int si  = 0;si<nsupporters-1;si++)
                fprintf(fp,"%i,",supporters->at(si).idx);
            fprintf(fp,"%i",supporters->at(nsupporters-1).idx);
            fprintf(fp,"];\n \t\t[");
            for (int si  = 0;si<nsupporters-1;si++)
                fprintf(fp,"%.3f,",supporters->at(si).val);
            fprintf(fp,"%.3f",supporters->at(nsupporters-1).val);
            fprintf(fp,"]];\n");
            
        }
        fclose(fp);
    }
};

// helper routine for comparing elements in priority queue
class Interval_comparison{public:
    bool operator() ( Interval* lhs,  Interval* rhs) const {
        return lhs->less(rhs);
    }
};

// data structure for priority queue
typedef std::priority_queue<Interval*,std::vector<Interval*>,Interval_comparison> Interval_heap;
#endif
