#include "mex.h"
#include "sparse_def.h"
#include <string.h>
#include <vector>
typedef std::vector<float> vec;
typedef std::vector<vec>   arr;
typedef std::vector<arr>   arr3;
typedef std::vector<std::vector<int> > cnfs;

inline float fastSqrt(const float x)
{
    union
    {
        int i;
        float x;
    } u;
    u.x = x;
    u.i = (1<<29) + (u.i >> 1) - (1<<22);
    u.x =       u.x + x/u.x;
    u.x = 0.25f*u.x + x/u.x;
    return u.x;
}
#define NB8

typedef unsigned char u8;
typedef unsigned int  u16;

#ifdef NB8
typedef u8 pidx;
#else
typedef u16  pidx;
#endif

typedef struct {const float *hogs; arr3 weights;    int dims[3]; int offsets[20][20]; int padh; int padv;} HOG_struct;

inline void copy_HOG(const mxArray *inm, HOG_struct *in,bool copy)
{
    in->hogs = (float*) mxGetPr(inm);
    int *ds = (int *)  mxGetDimensions(inm);
    in->dims[0] = ds[0];
    in->dims[1] = ds[1];
    in->dims[2] = ds[2];
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

/*
inline void erase_HOG(HOG_struct *in,bool copy)
{
    if (copy)
    {
        in->hogs = NULL;
        
    int ds[3];
    ds[0] = in->dims[0];
    ds[1] = in->dims[1];
    ds[2] = in->dims[2];
    for (int d2=0;d2<ds[2];d2++)
            for (int d1=0;d1<ds[1];d1++)
            {
                for (int d0=0;d0<ds[0];d0++)
                    erase(in->weights[d1][d2].start(),in->weights[d1][d2].end());
            }
        free(in->weights[d1]);
    }
    free(in->weights);
    
    
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
*/

typedef u8  mint;
typedef const float       cf;
#define my_min(a, b) ((a)<(b)? (a):(b))
#define my_max(a, b) ((a)>(b)? (a):(b))

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // usage:
    // scores = fast_lookup(indexes_at_pixels,values_for_models,Npixels,Ninbox,Nmodels,Nwords);
    // indexes_at_pixels: Ninbox x Npixels  array (Ninbox: indexes of box centered around current pixel)
    // values_for_models: Nwords x Ninbox x Nmodels
    // Cost that model (3) has if index (2) has word (1)
    
    int cnt = 0;
    const int FEAT_IND  = cnt++;    // letters  - HOG features
    const int T_IND     = cnt++;    // codebook - HOG filter
    const int S_IND     = cnt++;    // size of convolution
    const int V_IND     = cnt++;    // indexes of points
    const int H_IND     = cnt++;
    const int HS_IND    = cnt++;    // horizontal/vertical start & end
    const int HE_IND    = cnt++;
    const int VS_IND    = cnt++;
    const int VE_IND    = cnt++;
    const int CD_IND    = cnt++;    // horizontal-vertical displacement costs
    const int TR_IND    = cnt++;    // thresholds for deformation pruning
    
    // parameters for bounding code
    const int D_IND     = cnt++;        //distortion matrix
    const int N_IND     = cnt++;        //codebook energies
    const int E_IND     = cnt++;        //epsilon in bound
    
    // letters
    const pidx  *pixel_index  = (pidx *)  mxGetPr(prhs[FEAT_IND]);
    
    // useful image range
    const int* dims_im =  mxGetDimensions(prhs[FEAT_IND]);
    
    const bool do_hog = (nrhs<E_IND);
    const int in_hog = do_hog?1:0;
    
    const int sv    = (int)     dims_im[0+in_hog];
    const int sh    = (int)     dims_im[1+in_hog];
    
    const int h_s   = (int) mxGetScalar(prhs[HS_IND])-1;
    const int h_e   = (int) mxGetScalar(prhs[HE_IND])-1;
    const int v_s   = (int) mxGetScalar(prhs[VS_IND])-1;
    const int v_e   = (int) mxGetScalar(prhs[VE_IND])-1;
    
    int Npoints     = mxGetNumberOfElements(prhs[V_IND]);   // number of points
    const mint* vs  = (mint*) mxGetPr(prhs[V_IND]);         // point indexes
    const mint* hs  = (mint*) mxGetPr(prhs[H_IND]);
    
    // codebook
    const float *values_for_models  = (float *)  mxGetPr(prhs[T_IND]);
    
    // size of convolution kernel
    const int sz                    = (int)      mxGetScalar(prhs[S_IND]);
    
    // vertical and horizontal displacement costs
    const float *costs_vh            = (float *) mxGetPr(prhs[CD_IND]);
    const float *thrsh_pt            = (float *) mxGetPr(prhs[TR_IND]);
    
    
    // only meaningful for first round
    
    // relative locations of codeboook taps
    const mwSize   *cdbk_size = mxGetDimensions(prhs[T_IND]);
    const int      Nwords   = cdbk_size[0];
    
    // relative locations of image taps
    
   // int cn = 0;
    setup_codepos_and_offsets
            
    const float *dist_array;
    const float *nrm_loc;
    float fct_t = 1;
    //float epsilon;
    if (!do_hog)
    {
        dist_array   = (float *) mxGetPr(prhs[D_IND]);
        nrm_loc      = (float *) mxGetPr(prhs[N_IND]);
        if (mxGetNumberOfElements(prhs[0])>0)
            fct_t =  (float ) mxGetScalar(prhs[E_IND]);
    }
    else
    {
        nrm_loc = new float[36];
    }
    const float fct = fct_t;
    
    HOG_struct filter_hog;
    HOG_struct image_hog;
    
    float svft,shft;
    const float *hogim, *filtim;
    //const int
    if (do_hog)
    {
        copy_HOG(prhs[T_IND],   &filter_hog,true);
        copy_HOG(prhs[FEAT_IND],&image_hog ,false);
        svft =  filter_hog.dims[1];
        shft =  filter_hog.dims[2];
        hogim  = image_hog.hogs;
        filtim = filter_hog.hogs;
    }
    const int svf = svft;
    const int shf = shft;
    
    // boolean area indicating where we will perform the filtering
    int dims[2]         = {Npoints,1};
    plhs[0]             = mxCreateNumericArray(2, dims, mxSINGLE_CLASS, mxREAL);
    float *out_ind      = (float *) mxGetPr(plhs[0]);
    float *val_ind      = new float[sv*sh];
    
    pidx* px;
    pidx* py;
    const bool do3 = (nlhs>1);
    if (do3)
    {
        plhs[1]             = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
        plhs[2]             = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
        px = (pidx*) mxGetPr(plhs[1]);
        py = (pidx*) mxGetPr(plhs[2]);
    }
            
    for (int it=0;it<sv*sh; it++)
        val_ind[it] = 0;
    
    for (int it=0;it<Npoints; it++)
    {
        const int v0 = vs[it];
        const int h0 = hs[it];
        const int st_v_0 = v0 - sz;
        const int st_h_0 = h0 - sz;
        const int en_v_0 = v0 + sz+1;
        const int en_h_0 = h0 + sz+1;
        const int st_v = (st_v_0)<v_s?   v_s: st_v_0;
        const int st_h = (st_h_0)<h_s?   h_s: st_h_0;
        const int en_v = (en_v_0)>v_e?   v_e: en_v_0;
        const int en_h = (en_h_0)>h_e?   h_e: en_h_0;
        
        pidx xm, ym; 
        define_taps
       
        float   max_val = -10;
        const float thresh_def  = thrsh_pt[it];
        
        for (int h = st_h; h < en_h; h++) {
            const int hsv    = h*sv;
            bool jumped      = 1;
            float *ptr              = val_ind + hsv + st_v-1;
            const float *costs_u_t  = costs_vh + (h-st_h_0)*11 + (st_v - st_v_0);
            const float *hogi       = hogim    + 32*hsv;
            
            for (int v = st_v; v < en_v; v++) {
                ptr++;
                const float cost_def = *(costs_u_t++);
                if (cost_def<=thresh_def)
                    continue;
                
                float sum = *(ptr);
                float sum_dst;
                if (sum==0)
                {
                    if (do_hog)
                    {
                        const float *starting_point = hogi + 32*v;
                        for (int vin = 0; vin<svf;vin++)
                            for (int hin = 0; hin<shf;hin++)
                            {
                            const float *im = starting_point + image_hog.offsets[vin][hin];
                            const vec    wv = filter_hog.weights[vin][hin];
                            sum +=    wv[0]*im[0]+wv[1]*im[1]+wv[2]*im[2]+wv[3]*im[3]+wv[4]*im[4]+wv[5]*im[5]+wv[6]*im[6]+wv[7]*im[7]+wv[8]*im[8]\
                                    +wv[9]*im[9]+wv[10]*im[10]+wv[11]*im[11]+wv[12]*im[12]+wv[13]*im[13]+wv[14]*im[14]+wv[15]*im[15]\
                                    +wv[16]*im[16]+wv[17]*im[17]+wv[18]*im[18]+wv[19]*im[19]+wv[20]*im[20]+wv[21]*im[21]+wv[22]*im[22]\
                                    +wv[23]*im[23]+wv[24]*im[24]+wv[25]*im[25]+wv[26]*im[26]+wv[27]*im[27]+wv[28]*im[28]+wv[29]*im[29]\
                                    +wv[30]*im[30]+wv[31]*im[31];
                            }
                        
                        (*ptr) = sum;
                    }
                    else
                    {
                        if (jumped)
                        {
                            const pidx* pix_offset = pixel_index + hsv + v;
                            const float* dst_offset = dist_array  + hsv + v;
                            update_taps
                        }
                        sum     = get_sum;
                        sum_dst = get_dst_sum;
                        sum     += fct*fastSqrt(sum_dst);
                        *(ptr) = sum;
                        jumped = 0;
                    }
                }
                else
                    jumped = 1;
                
                sum  += cost_def;
                if (sum>max_val)
                {
                    max_val = sum;
                    if (do3)
                    {
                        xm = h;
                        ym = v;
                    }   
                }
            }
            
        }
        *(out_ind++) = max_val;
        if (do3)
        {
            *(px++) = xm;
            *(py++) = ym;
        }
    }
    if (do_hog)
		delete []nrm_loc;
    delete []val_ind;
}