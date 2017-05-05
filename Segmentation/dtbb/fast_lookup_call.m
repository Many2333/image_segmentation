function [inpr,dst]  =fast_lookup_call(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err)
%do_fast = exist('Err');
%if do_fast
Err = single(Err);
switch Nh,
    case 1,
        [inpr,dst]                    = fast_8bit_1(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 4,
        [inpr,dst]                    = fast_8bit_4(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 5,
        [inpr,dst]                    = fast_8bit_5(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 6,
        [inpr,dst]                    = fast_8bit_6(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 7,
        [inpr,dst]                    = fast_8bit_7(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 8,
        [inpr,dst]                    = fast_8bit_8(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 9,
        [inpr,dst]                    = fast_8bit_9(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 10,
        [inpr,dst]                    = fast_8bit_10(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 11,
        [inpr,dst]                    = fast_8bit_11(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 12,
        [inpr,dst]                    = fast_8bit_12(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 13,
        [inpr,dst]                    = fast_8bit_13(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 14,
        [inpr,dst]                    = fast_8bit_14(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 15,
        [inpr,dst]                    = fast_8bit_15(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
    case 16,
        [inpr,dst]                    = fast_8bit_16(feat,t,Nv,Nh,h_s,h_e,v_s,v_e,d,e,Err);
end
