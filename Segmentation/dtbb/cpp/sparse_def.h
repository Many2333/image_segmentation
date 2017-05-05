#define setup_codepos_and_offsets \
const int offsets0 =  0*sv + 0;\
const int offsets1 =  0*sv + 1;\
const int offsets2 =  0*sv + 2;\
const int offsets3 =  0*sv + 3;\
const int offsets4 =  0*sv + 4;\
const int offsets5 =  0*sv + 5;\
const int offsets6 =  1*sv + 0;\
const int offsets7 =  1*sv + 1;\
const int offsets8 =  1*sv + 2;\
const int offsets9 =  1*sv + 3;\
const int offsets10 =  1*sv + 4;\
const int offsets11 =  1*sv + 5;\
const int offsets12 =  2*sv + 0;\
const int offsets13 =  2*sv + 1;\
const int offsets14 =  2*sv + 2;\
const int offsets15 =  2*sv + 3;\
const int offsets16 =  2*sv + 4;\
const int offsets17 =  2*sv + 5;\
const int offsets18 =  3*sv + 0;\
const int offsets19 =  3*sv + 1;\
const int offsets20 =  3*sv + 2;\
const int offsets21 =  3*sv + 3;\
const int offsets22 =  3*sv + 4;\
const int offsets23 =  3*sv + 5;\
const int offsets24 =  4*sv + 0;\
const int offsets25 =  4*sv + 1;\
const int offsets26 =  4*sv + 2;\
const int offsets27 =  4*sv + 3;\
const int offsets28 =  4*sv + 4;\
const int offsets29 =  4*sv + 5;\
const int offsets30 =  5*sv + 0;\
const int offsets31 =  5*sv + 1;\
const int offsets32 =  5*sv + 2;\
const int offsets33 =  5*sv + 3;\
const int offsets34 =  5*sv + 4;\
const int offsets35 =  5*sv + 5;\
const float* values0  = values_for_models +  0*Nwords;\
const float* values1  = values_for_models +  1*Nwords;\
const float* values2  = values_for_models +  2*Nwords;\
const float* values3  = values_for_models +  3*Nwords;\
const float* values4  = values_for_models +  4*Nwords;\
const float* values5  = values_for_models +  5*Nwords;\
const float* values6  = values_for_models +  6*Nwords;\
const float* values7  = values_for_models +  7*Nwords;\
const float* values8  = values_for_models +  8*Nwords;\
const float* values9  = values_for_models +  9*Nwords;\
const float* values10  = values_for_models +  10*Nwords;\
const float* values11  = values_for_models +  11*Nwords;\
const float* values12  = values_for_models +  12*Nwords;\
const float* values13  = values_for_models +  13*Nwords;\
const float* values14  = values_for_models +  14*Nwords;\
const float* values15  = values_for_models +  15*Nwords;\
const float* values16  = values_for_models +  16*Nwords;\
const float* values17  = values_for_models +  17*Nwords;\
const float* values18  = values_for_models +  18*Nwords;\
const float* values19  = values_for_models +  19*Nwords;\
const float* values20  = values_for_models +  20*Nwords;\
const float* values21  = values_for_models +  21*Nwords;\
const float* values22  = values_for_models +  22*Nwords;\
const float* values23  = values_for_models +  23*Nwords;\
const float* values24  = values_for_models +  24*Nwords;\
const float* values25  = values_for_models +  25*Nwords;\
const float* values26  = values_for_models +  26*Nwords;\
const float* values27  = values_for_models +  27*Nwords;\
const float* values28  = values_for_models +  28*Nwords;\
const float* values29  = values_for_models +  29*Nwords;\
const float* values30  = values_for_models +  30*Nwords;\
const float* values31  = values_for_models +  31*Nwords;\
const float* values32  = values_for_models +  32*Nwords;\
const float* values33  = values_for_models +  33*Nwords;\
const float* values34  = values_for_models +  34*Nwords;\
const float* values35  = values_for_models +  35*Nwords;\

#define define_taps \
const pidx* tap0;\
const pidx* tap1;\
const pidx* tap2;\
const pidx* tap3;\
const pidx* tap4;\
const pidx* tap5;\
const pidx* tap6;\
const pidx* tap7;\
const pidx* tap8;\
const pidx* tap9;\
const pidx* tap10;\
const pidx* tap11;\
const pidx* tap12;\
const pidx* tap13;\
const pidx* tap14;\
const pidx* tap15;\
const pidx* tap16;\
const pidx* tap17;\
const pidx* tap18;\
const pidx* tap19;\
const pidx* tap20;\
const pidx* tap21;\
const pidx* tap22;\
const pidx* tap23;\
const pidx* tap24;\
const pidx* tap25;\
const pidx* tap26;\
const pidx* tap27;\
const pidx* tap28;\
const pidx* tap29;\
const pidx* tap30;\
const pidx* tap31;\
const pidx* tap32;\
const pidx* tap33;\
const pidx* tap34;\
const pidx* tap35;\
const float* dtap0;\
const float* dtap1;\
const float* dtap2;\
const float* dtap3;\
const float* dtap4;\
const float* dtap5;\
const float* dtap6;\
const float* dtap7;\
const float* dtap8;\
const float* dtap9;\
const float* dtap10;\
const float* dtap11;\
const float* dtap12;\
const float* dtap13;\
const float* dtap14;\
const float* dtap15;\
const float* dtap16;\
const float* dtap17;\
const float* dtap18;\
const float* dtap19;\
const float* dtap20;\
const float* dtap21;\
const float* dtap22;\
const float* dtap23;\
const float* dtap24;\
const float* dtap25;\
const float* dtap26;\
const float* dtap27;\
const float* dtap28;\
const float* dtap29;\
const float* dtap30;\
const float* dtap31;\
const float* dtap32;\
const float* dtap33;\
const float* dtap34;\
const float* dtap35;\
const float ntap0 = nrm_loc[0];\
const float ntap1 = nrm_loc[1];\
const float ntap2 = nrm_loc[2];\
const float ntap3 = nrm_loc[3];\
const float ntap4 = nrm_loc[4];\
const float ntap5 = nrm_loc[5];\
const float ntap6 = nrm_loc[6];\
const float ntap7 = nrm_loc[7];\
const float ntap8 = nrm_loc[8];\
const float ntap9 = nrm_loc[9];\
const float ntap10 = nrm_loc[10];\
const float ntap11 = nrm_loc[11];\
const float ntap12 = nrm_loc[12];\
const float ntap13 = nrm_loc[13];\
const float ntap14 = nrm_loc[14];\
const float ntap15 = nrm_loc[15];\
const float ntap16 = nrm_loc[16];\
const float ntap17 = nrm_loc[17];\
const float ntap18 = nrm_loc[18];\
const float ntap19 = nrm_loc[19];\
const float ntap20 = nrm_loc[20];\
const float ntap21 = nrm_loc[21];\
const float ntap22 = nrm_loc[22];\
const float ntap23 = nrm_loc[23];\
const float ntap24 = nrm_loc[24];\
const float ntap25 = nrm_loc[25];\
const float ntap26 = nrm_loc[26];\
const float ntap27 = nrm_loc[27];\
const float ntap28 = nrm_loc[28];\
const float ntap29 = nrm_loc[29];\
const float ntap30 = nrm_loc[30];\
const float ntap31 = nrm_loc[31];\
const float ntap32 = nrm_loc[32];\
const float ntap33 = nrm_loc[33];\
const float ntap34 = nrm_loc[34];\
const float ntap35 = nrm_loc[35];\

#define update_taps \
tap0 = pix_offset + offsets0;\
tap1 = pix_offset + offsets1;\
tap2 = pix_offset + offsets2;\
tap3 = pix_offset + offsets3;\
tap4 = pix_offset + offsets4;\
tap5 = pix_offset + offsets5;\
tap6 = pix_offset + offsets6;\
tap7 = pix_offset + offsets7;\
tap8 = pix_offset + offsets8;\
tap9 = pix_offset + offsets9;\
tap10 = pix_offset + offsets10;\
tap11 = pix_offset + offsets11;\
tap12 = pix_offset + offsets12;\
tap13 = pix_offset + offsets13;\
tap14 = pix_offset + offsets14;\
tap15 = pix_offset + offsets15;\
tap16 = pix_offset + offsets16;\
tap17 = pix_offset + offsets17;\
tap18 = pix_offset + offsets18;\
tap19 = pix_offset + offsets19;\
tap20 = pix_offset + offsets20;\
tap21 = pix_offset + offsets21;\
tap22 = pix_offset + offsets22;\
tap23 = pix_offset + offsets23;\
tap24 = pix_offset + offsets24;\
tap25 = pix_offset + offsets25;\
tap26 = pix_offset + offsets26;\
tap27 = pix_offset + offsets27;\
tap28 = pix_offset + offsets28;\
tap29 = pix_offset + offsets29;\
tap30 = pix_offset + offsets30;\
tap31 = pix_offset + offsets31;\
tap32 = pix_offset + offsets32;\
tap33 = pix_offset + offsets33;\
tap34 = pix_offset + offsets34;\
tap35 = pix_offset + offsets35;\
dtap0 = dst_offset + offsets0;\
dtap1 = dst_offset + offsets1;\
dtap2 = dst_offset + offsets2;\
dtap3 = dst_offset + offsets3;\
dtap4 = dst_offset + offsets4;\
dtap5 = dst_offset + offsets5;\
dtap6 = dst_offset + offsets6;\
dtap7 = dst_offset + offsets7;\
dtap8 = dst_offset + offsets8;\
dtap9 = dst_offset + offsets9;\
dtap10 = dst_offset + offsets10;\
dtap11 = dst_offset + offsets11;\
dtap12 = dst_offset + offsets12;\
dtap13 = dst_offset + offsets13;\
dtap14 = dst_offset + offsets14;\
dtap15 = dst_offset + offsets15;\
dtap16 = dst_offset + offsets16;\
dtap17 = dst_offset + offsets17;\
dtap18 = dst_offset + offsets18;\
dtap19 = dst_offset + offsets19;\
dtap20 = dst_offset + offsets20;\
dtap21 = dst_offset + offsets21;\
dtap22 = dst_offset + offsets22;\
dtap23 = dst_offset + offsets23;\
dtap24 = dst_offset + offsets24;\
dtap25 = dst_offset + offsets25;\
dtap26 = dst_offset + offsets26;\
dtap27 = dst_offset + offsets27;\
dtap28 = dst_offset + offsets28;\
dtap29 = dst_offset + offsets29;\
dtap30 = dst_offset + offsets30;\
dtap31 = dst_offset + offsets31;\
dtap32 = dst_offset + offsets32;\
dtap33 = dst_offset + offsets33;\
dtap34 = dst_offset + offsets34;\
dtap35 = dst_offset + offsets35;\

#define get_sum \
values0[*(tap0++)] + values1[*(tap1++)] + values2[*(tap2++)] + values3[*(tap3++)] + values4[*(tap4++)] + values5[*(tap5++)] + values6[*(tap6++)] + values7[*(tap7++)] + values8[*(tap8++)] + values9[*(tap9++)] + values10[*(tap10++)] + values11[*(tap11++)] + values12[*(tap12++)] + values13[*(tap13++)] + values14[*(tap14++)] + values15[*(tap15++)] + values16[*(tap16++)] + values17[*(tap17++)] + values18[*(tap18++)] + values19[*(tap19++)] + values20[*(tap20++)] + values21[*(tap21++)] + values22[*(tap22++)] + values23[*(tap23++)] + values24[*(tap24++)] + values25[*(tap25++)] + values26[*(tap26++)] + values27[*(tap27++)] + values28[*(tap28++)] + values29[*(tap29++)] + values30[*(tap30++)] + values31[*(tap31++)] + values32[*(tap32++)] + values33[*(tap33++)] + values34[*(tap34++)] + values35[*(tap35++)]

#define get_dst_sum \
*(dtap0++)*ntap0 + *(dtap1++)*ntap1 + *(dtap2++)*ntap2 + *(dtap3++)*ntap3 + *(dtap4++)*ntap4 + *(dtap5++)*ntap5 + *(dtap6++)*ntap6 + *(dtap7++)*ntap7 + *(dtap8++)*ntap8 + *(dtap9++)*ntap9 + *(dtap10++)*ntap10 + *(dtap11++)*ntap11 + *(dtap12++)*ntap12 + *(dtap13++)*ntap13 + *(dtap14++)*ntap14 + *(dtap15++)*ntap15 + *(dtap16++)*ntap16 + *(dtap17++)*ntap17 + *(dtap18++)*ntap18 + *(dtap19++)*ntap19 + *(dtap20++)*ntap20 + *(dtap21++)*ntap21 + *(dtap22++)*ntap22 + *(dtap23++)*ntap23 + *(dtap24++)*ntap24 + *(dtap25++)*ntap25 + *(dtap26++)*ntap26 + *(dtap27++)*ntap27 + *(dtap28++)*ntap28 + *(dtap29++)*ntap29 + *(dtap30++)*ntap30 + *(dtap31++)*ntap31 + *(dtap32++)*ntap32 + *(dtap33++)*ntap33 + *(dtap34++)*ntap34 + *(dtap35++)*ntap35
