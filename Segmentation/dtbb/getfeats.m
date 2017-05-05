
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