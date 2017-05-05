function [kd_struct,times,grh,grv,p,sz0,sz2,ng,grh0,grv0] = get_unary(pyra,level,t,sz_filter,bw,offsets,bs,doa,lookup_table,nr,fct_err)
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2


get_kdtree  = exist('bw','var');

% doa: do approximation
if ~exist('doa','var'),
    doa = 0;
end
is_root = (exist('bs','var'))&&(~isempty(bs));

%% get image features
if doa
    feat = pyra.i{level};
    d    = pyra.d{level};
else
    feat = pyra.feat{level};
end

%% determine area of operation 
%% h_s,h_e,v_s,v_e: pyramid coordinates of the area being processed
%% grh,grv        : grid of points within it.
%% ng             : turns pyramid into image coordinates (discounts padding effects)
%% sz0            : dimensions of the pyramid domain being processed
%% sz2:           : as above, but clipped to the closest power of two.

[h_s,h_e,v_s,v_e,grfh,grfv,ng] = get_domain(pyra,feat,sz_filter);
[grh,grv,sz0,sz2,idx]          = get_domain_pts(h_s,h_e,v_s,v_e,feat);

tidx = tic;
if doa,
    inpr  = fast_lookup_call(uint8(feat),lookup_table,sz_filter(1),sz_filter(2),h_s,h_e,v_s,v_e,[],[],[]);
else
    inpr  = (getfeats(feat,idx,grfh,grfv)*t)';
end

grh0 = grh; grv0 = grv; 
% construct KD-tree using image coordinates (so as to discount pyramid
% effects)
% turn pyramid coordinates into image coordinates (shift backwards)
% (padx+1, pady+1) will become (0,0)
grh         = grh + ng(1);
grv         = grv + ng(2);

scf         = 1 + (~is_root);  %spacing of points: 1 for low res, 2 for high-res points

% Indicate where (grh=1,grv=1) should get mapped to. 
ng          = ng + [h_s,v_s]   -1;

if ~get_kdtree,
    kd_struct = []; p = inpr;
    return; 
end

if is_root,
    for k=1:length(bs),
        inpr(k,:) = inpr(k,:) + bs(k);
    end
end
if doa,
    derr        = nr*(getfeats(d,idx,grfh,grfv))';
    slack       = sqrt(derr);
    inprd       = inpr - fct_err*slack;
    inpr        = inpr + fct_err*slack;
    p   = cat(1,inpr,inprd,inprd);
    NR  = 3;
else
    p   = cat(1,inpr,inpr);
    NR  = 2;
end
times(1) = toc(tidx);

times_kd = tic;
kd_struct   = mex_multitree([grh/scf;grv/scf],p,offsets,bw,sz0,sz2,NR,scf,ng(1),ng(2));
times(2) = toc(times_kd);

function [blocks]  = getfeats(feats,idxs,grfh,grfv)
%blocks  = getfeats(feats,idxs,grfh,grfv);
%
%% crops the hog cells used for convolution.
%
%% idx: vector of image indexes
%% fil_u.grfh: vector of horizontal offsets used for convolution
%% fil_u.grfv: vector of vertical   offsets used for convolution

[sv(1),sv(2),sv(3)] = size(feats);
nun             = length(idxs);

%% compute the (image-dependent) offsets needed to crop cells :
%% (horizontal, vertical) offset -> index offset
offsets         = uint16(grfh*sv(1)          + grfv);
idx_r           = repmat(idxs',[1,length(offsets)]) + repmat(offsets,[nun,1]);
feat_reshaped   = single(reshape(feats,[sv(1)*sv(2),sv(3)]));

nels            = numel(grfh)*sv(3);
blocks          = reshape(feat_reshaped(idx_r,:),[nun,nels]);
