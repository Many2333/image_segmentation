#include "mex.h"
#include "dense_def.h"

#ifdef APPLE
#include <inttypes.h>
#endif

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

typedef unsigned char u8;
typedef unsigned int  u16;

#ifdef NB8
typedef u8 pidx;
#else
typedef u16 pidx;
#endif
typedef u16  mint;

typedef const float       cf;
#define my_min(a, b) ((a)<(b)? (a):(b))
#define my_max(a, b) ((a)>(b)? (a):(b))
#define mycase(a) {case a: values_for_model = values_for_model_inbox + a*Nm;}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    // usage:
    // scores = fast_lookup(indexes_at_pixels,values_for_models,Npixels,Ninbox,Nmodels,Nwords);
    // indexes_at_pixels: Ninbox x Npixels  array (Ninbox: indexes of box centered around current pixel)
    // values_for_models: Nwords x Ninbox x Nmodels
    // Cost that model (3) has if index (2) has word (1)
	//   [pp,pm]      = fast_inner6(feat,t,h_s,h_e,v_s,v_e,grfh,Nv,Nwords,d,nrm,perr);

    int cnt = 0;
    const int FEAT_IND  = cnt++;
    const int T_IND     = cnt++;
    const int NV_IND    = cnt++;
    const int NH_IND    = cnt++;
    const int HS_IND    = cnt++;
    const int HE_IND    = cnt++;
    const int VS_IND    = cnt++;
    const int VE_IND    = cnt++;
    const int D_IND     = cnt++;        //distortion matrix
    const int N_IND     = cnt++;        //codebook energies 
    const int E_IND     = cnt++;        //epsilon in bound
    //const int NR_IND    = cnt++;
    
    
    // get image letters & associated info
    //mxClassID nbits = mxGetClassID(prhs[FEAT_IND]);
    const pidx  *pixel_index  = (pidx *)  mxGetPr(prhs[FEAT_IND]);
    const int sv              = (int)     mxGetM(prhs[FEAT_IND]);

    const float *values_for_models = (float *)  mxGetPr(prhs[T_IND]);
    
    const int Nv      = (int) mxGetScalar(prhs[NV_IND]);
    //const int Nh      = (int) mxGetScalar(prhs[NH_IND]);
    
    setup_offsets
    
    // output dimensions
    const int h_s  = (int) mxGetScalar(prhs[HS_IND])-1;
    const int h_e  = (int) mxGetScalar(prhs[HE_IND])-1;
    const int v_s  = (int) mxGetScalar(prhs[VS_IND])-1;
    const int v_e  = (int) mxGetScalar(prhs[VE_IND])-1;
    const int Npixels = (h_e - h_s+1)*(v_e-v_s+1);
    

    const mwSize *cdbk_size = mxGetDimensions(prhs[T_IND]);
    const int Nmodels_all   = cdbk_size[0];   
    const int Nwords        = cdbk_size[1];
    
    const int Nmodels       = Nmodels_all/Nv;
    const int NmNw          = Nmodels_all*Nwords;
    
    bool get_distortion=0;
    //const float *norms;
    //const float *distortions;
    //const float *norm_index;
    //float pe = 0;
    
     // create output array
    int dims[2];
    if (Nmodels==1)
    {   
        dims[0] = v_e - v_s + 1;
        dims[1] = h_e - h_s + 1;
    }
    else
    {
        dims[0] = Nmodels;
        dims[1] = Npixels;
    }
    
    plhs[0]             = mxCreateNumericArray(2, dims, mxSINGLE_CLASS, mxREAL);
    plhs[1]             = mxCreateNumericArray(2, dims, mxSINGLE_CLASS, mxREAL);
    
    float *out_ind      = (float *) mxGetPr(plhs[0]);
    float *dst_ind      = (float *) mxGetPr(plhs[1]);

    // distortion-related code 
    get_distortion = 0;
    const float *dist_array;
    const float *nrm_loc;
    float fct = 1;
    if (nrhs>D_IND)
    {
        dist_array   = (float *) mxGetPr(prhs[D_IND]);
        nrm_loc      = (float *) mxGetPr(prhs[N_IND]);
        if (mxGetNumberOfElements(prhs[D_IND])>0)
        {
            get_distortion =1;
            fct =  (float ) mxGetScalar(prhs[E_IND]);
        }
    }

    code_setup
            
   for (int h  = h_s; h<=h_e; h++)
    {
        int cn = 0;
        const int hsv = h*sv;
        const pidx  *pix_h = pixel_index   + hsv;
        const float *dix_h = dist_array    + hsv;
        //const float *nrm_h = norm_index;

        const int offset_h =  (h-h_s)*Nmodels*(v_e-v_s+1);
        setup_distortion
                
        for (int v  = v_s; v<=(v_e +Nv); v++)
        {
          	const int df        = v +1 - v_s;
            const int off       = my_min(df,Nv);
            const int nback     = Nmodels*off;
            const int nend      = Nmodels*my_max(v- v_e,0);
            const pidx *pix_loc  = pix_h + v;
            const int added     = Nmodels  + offset_h + cn - Nmodels_all;
            float *save_ind     = out_ind + added;
                
            code_nval
            const int end_loop = Nmodels_all - nend;
            
            // Nmodels_all: this is equal to the product of Nmodels with
            if (get_distortion)
            {
                const float *dix_loc = dix_h + v;
                update_distortion_taps
                float *save_dst     = dst_ind + added;
                for (int s = (Nmodels_all  - nback);s<(end_loop); s++)
                    save_dst[s] += sum_distortion;
            }
            
            for (int s = (Nmodels_all  - nback);s<(end_loop); s++) 
               save_ind[s] += sum_nval;
            
            cn += Nmodels;
        }
    }
    
    if (fct!=1)
    {
    cf fct_u = fct;
    for (int h  = h_s; h<=h_e; h++)
    {
        const int offset_h =  (h-h_s)*Nmodels*(v_e-v_s+1);
        float *save_ind  = out_ind + offset_h;
        cf *save_dst     = dst_ind + offset_h;
        for (int v  = v_s; v<=(v_e); v++)
            *(save_ind++) += fct_u*fastSqrt(*save_dst++);
    }
    }
}
