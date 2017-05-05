#ifdef NH1 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\

#define sum_nval values0[s]  

#define setup_offsets \
const int off0 = 0*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\

#define sum_distortion nrm0[s]*(dst0) 

#endif 

#ifdef NH2 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\

#define sum_nval values0[s] + values1[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) 

#endif 

#ifdef NH4 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) 

#endif 

#ifdef NH5 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) 

#endif 

#ifdef NH6 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) 

#endif 

#ifdef NH7 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) 

#endif 

#ifdef NH8 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) 

#endif 

#ifdef NH9 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) 

#endif 

#ifdef NH10 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) 

#endif 

#ifdef NH11 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) 

#endif 

#ifdef NH12 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\
cf *values11 	 = values_for_models + Nmodels_all*(*(pix_loc + off11)) 	 + NmNw11;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s] + values11[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\
const int off11 = 11*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\
cf *nrm11 = nrm_loc + Nma11;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\
cf dst11 = *(dix_loc + off11);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) + nrm11[s]*(dst11) 

#endif 

#ifdef NH13 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\
cf *values11 	 = values_for_models + Nmodels_all*(*(pix_loc + off11)) 	 + NmNw11;\
cf *values12 	 = values_for_models + Nmodels_all*(*(pix_loc + off12)) 	 + NmNw12;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s] + values11[s] + values12[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\
const int off11 = 11*sv;\
const int off12 = 12*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\
cf *nrm11 = nrm_loc + Nma11;\
cf *nrm12 = nrm_loc + Nma12;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\
cf dst11 = *(dix_loc + off11);\
cf dst12 = *(dix_loc + off12);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) + nrm11[s]*(dst11) + nrm12[s]*(dst12) 

#endif 

#ifdef NH14 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\
cf *values11 	 = values_for_models + Nmodels_all*(*(pix_loc + off11)) 	 + NmNw11;\
cf *values12 	 = values_for_models + Nmodels_all*(*(pix_loc + off12)) 	 + NmNw12;\
cf *values13 	 = values_for_models + Nmodels_all*(*(pix_loc + off13)) 	 + NmNw13;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s] + values11[s] + values12[s] + values13[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\
const int off11 = 11*sv;\
const int off12 = 12*sv;\
const int off13 = 13*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\
cf *nrm11 = nrm_loc + Nma11;\
cf *nrm12 = nrm_loc + Nma12;\
cf *nrm13 = nrm_loc + Nma13;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\
cf dst11 = *(dix_loc + off11);\
cf dst12 = *(dix_loc + off12);\
cf dst13 = *(dix_loc + off13);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) + nrm11[s]*(dst11) + nrm12[s]*(dst12) + nrm13[s]*(dst13) 

#endif 

#ifdef NH15 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\
cf *values11 	 = values_for_models + Nmodels_all*(*(pix_loc + off11)) 	 + NmNw11;\
cf *values12 	 = values_for_models + Nmodels_all*(*(pix_loc + off12)) 	 + NmNw12;\
cf *values13 	 = values_for_models + Nmodels_all*(*(pix_loc + off13)) 	 + NmNw13;\
cf *values14 	 = values_for_models + Nmodels_all*(*(pix_loc + off14)) 	 + NmNw14;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s] + values11[s] + values12[s] + values13[s] + values14[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\
const int off11 = 11*sv;\
const int off12 = 12*sv;\
const int off13 = 13*sv;\
const int off14 = 14*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\
cf *nrm11 = nrm_loc + Nma11;\
cf *nrm12 = nrm_loc + Nma12;\
cf *nrm13 = nrm_loc + Nma13;\
cf *nrm14 = nrm_loc + Nma14;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\
cf dst11 = *(dix_loc + off11);\
cf dst12 = *(dix_loc + off12);\
cf dst13 = *(dix_loc + off13);\
cf dst14 = *(dix_loc + off14);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) + nrm11[s]*(dst11) + nrm12[s]*(dst12) + nrm13[s]*(dst13) + nrm14[s]*(dst14) 

#endif 

#ifdef NH16 
#define code_nval \
cf *values0 	 = values_for_models + Nmodels_all*(*(pix_loc + off0)) 	 + NmNw0;\
cf *values1 	 = values_for_models + Nmodels_all*(*(pix_loc + off1)) 	 + NmNw1;\
cf *values2 	 = values_for_models + Nmodels_all*(*(pix_loc + off2)) 	 + NmNw2;\
cf *values3 	 = values_for_models + Nmodels_all*(*(pix_loc + off3)) 	 + NmNw3;\
cf *values4 	 = values_for_models + Nmodels_all*(*(pix_loc + off4)) 	 + NmNw4;\
cf *values5 	 = values_for_models + Nmodels_all*(*(pix_loc + off5)) 	 + NmNw5;\
cf *values6 	 = values_for_models + Nmodels_all*(*(pix_loc + off6)) 	 + NmNw6;\
cf *values7 	 = values_for_models + Nmodels_all*(*(pix_loc + off7)) 	 + NmNw7;\
cf *values8 	 = values_for_models + Nmodels_all*(*(pix_loc + off8)) 	 + NmNw8;\
cf *values9 	 = values_for_models + Nmodels_all*(*(pix_loc + off9)) 	 + NmNw9;\
cf *values10 	 = values_for_models + Nmodels_all*(*(pix_loc + off10)) 	 + NmNw10;\
cf *values11 	 = values_for_models + Nmodels_all*(*(pix_loc + off11)) 	 + NmNw11;\
cf *values12 	 = values_for_models + Nmodels_all*(*(pix_loc + off12)) 	 + NmNw12;\
cf *values13 	 = values_for_models + Nmodels_all*(*(pix_loc + off13)) 	 + NmNw13;\
cf *values14 	 = values_for_models + Nmodels_all*(*(pix_loc + off14)) 	 + NmNw14;\
cf *values15 	 = values_for_models + Nmodels_all*(*(pix_loc + off15)) 	 + NmNw15;\

#define sum_nval values0[s] + values1[s] + values2[s] + values3[s] + values4[s] + values5[s] + values6[s] + values7[s] + values8[s] + values9[s] + values10[s] + values11[s] + values12[s] + values13[s] + values14[s] + values15[s]  

#define setup_offsets \
const int off0 = 0*sv;\
const int off1 = 1*sv;\
const int off2 = 2*sv;\
const int off3 = 3*sv;\
const int off4 = 4*sv;\
const int off5 = 5*sv;\
const int off6 = 6*sv;\
const int off7 = 7*sv;\
const int off8 = 8*sv;\
const int off9 = 9*sv;\
const int off10 = 10*sv;\
const int off11 = 11*sv;\
const int off12 = 12*sv;\
const int off13 = 13*sv;\
const int off14 = 14*sv;\
const int off15 = 15*sv;\

#define setup_distortion \
cf *nrm0 = nrm_loc + Nma0;\
cf *nrm1 = nrm_loc + Nma1;\
cf *nrm2 = nrm_loc + Nma2;\
cf *nrm3 = nrm_loc + Nma3;\
cf *nrm4 = nrm_loc + Nma4;\
cf *nrm5 = nrm_loc + Nma5;\
cf *nrm6 = nrm_loc + Nma6;\
cf *nrm7 = nrm_loc + Nma7;\
cf *nrm8 = nrm_loc + Nma8;\
cf *nrm9 = nrm_loc + Nma9;\
cf *nrm10 = nrm_loc + Nma10;\
cf *nrm11 = nrm_loc + Nma11;\
cf *nrm12 = nrm_loc + Nma12;\
cf *nrm13 = nrm_loc + Nma13;\
cf *nrm14 = nrm_loc + Nma14;\
cf *nrm15 = nrm_loc + Nma15;\

#define update_distortion_taps \
cf dst0 = *(dix_loc + off0);\
cf dst1 = *(dix_loc + off1);\
cf dst2 = *(dix_loc + off2);\
cf dst3 = *(dix_loc + off3);\
cf dst4 = *(dix_loc + off4);\
cf dst5 = *(dix_loc + off5);\
cf dst6 = *(dix_loc + off6);\
cf dst7 = *(dix_loc + off7);\
cf dst8 = *(dix_loc + off8);\
cf dst9 = *(dix_loc + off9);\
cf dst10 = *(dix_loc + off10);\
cf dst11 = *(dix_loc + off11);\
cf dst12 = *(dix_loc + off12);\
cf dst13 = *(dix_loc + off13);\
cf dst14 = *(dix_loc + off14);\
cf dst15 = *(dix_loc + off15);\

#define sum_distortion nrm0[s]*(dst0) + nrm1[s]*(dst1) + nrm2[s]*(dst2) + nrm3[s]*(dst3) + nrm4[s]*(dst4) + nrm5[s]*(dst5) + nrm6[s]*(dst6) + nrm7[s]*(dst7) + nrm8[s]*(dst8) + nrm9[s]*(dst9) + nrm10[s]*(dst10) + nrm11[s]*(dst11) + nrm12[s]*(dst12) + nrm13[s]*(dst13) + nrm14[s]*(dst14) + nrm15[s]*(dst15) 

#endif 

#define code_setup \
const int NmNw0 = 0*NmNw;\
const int Nma0  = 0*Nmodels_all;\
const int NmNw1 = 1*NmNw;\
const int Nma1  = 1*Nmodels_all;\
const int NmNw2 = 2*NmNw;\
const int Nma2  = 2*Nmodels_all;\
const int NmNw3 = 3*NmNw;\
const int Nma3  = 3*Nmodels_all;\
const int NmNw4 = 4*NmNw;\
const int Nma4  = 4*Nmodels_all;\
const int NmNw5 = 5*NmNw;\
const int Nma5  = 5*Nmodels_all;\
const int NmNw6 = 6*NmNw;\
const int Nma6  = 6*Nmodels_all;\
const int NmNw7 = 7*NmNw;\
const int Nma7  = 7*Nmodels_all;\
const int NmNw8 = 8*NmNw;\
const int Nma8  = 8*Nmodels_all;\
const int NmNw9 = 9*NmNw;\
const int Nma9  = 9*Nmodels_all;\
const int NmNw10 = 10*NmNw;\
const int Nma10  = 10*Nmodels_all;\
const int NmNw11 = 11*NmNw;\
const int Nma11  = 11*Nmodels_all;\
const int NmNw12 = 12*NmNw;\
const int Nma12  = 12*Nmodels_all;\
const int NmNw13 = 13*NmNw;\
const int Nma13  = 13*Nmodels_all;\
const int NmNw14 = 14*NmNw;\
const int Nma14  = 14*Nmodels_all;\
const int NmNw15 = 15*NmNw;\
const int Nma15  = 15*Nmodels_all;\

