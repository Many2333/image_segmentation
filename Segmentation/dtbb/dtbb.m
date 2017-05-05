function [dets,boxes,is,times,tr,domain_kdtree] = dtbb(im,object_model,thr,nmax,fct_err)
% [dets,boxes,is,times,tr,domain_kdtree] = dtbb(im,object_model,thr,nmax,fct_err)
% fast object detection using dual-tree branch&bound ; see demo_all.m for
% usage
% Inputs:
% im:           input image 
% object_model: data structure containing model information
% thr:          no hypothesis below this threshold will be retrieved
% nmax:         max # of detections. Set to negative value to get all
% detections above threshold (cascaded detection is used for that). 
% Set to integer number, k, to get at most k configurations (BB is used for
% that). BB will stop if the upper bound of an interval is below thr
% fct_err:      If defined, non-empty, and bounded means that we are
%               using a lookup-based approximation to compute the scores
%               (PnA 2012 paper). Set to sqrt(1/p_error)
%               
%
% Outputs:
% Detection data:
% dets,boxes,is:    identical with the outputs of imgdetect (Felzenszwalb & co)
% Timing data
% times:            (1): pyramid (2): unary (3): kdtrees (4:end): search
% Display data:
% tr:               keeps track of visited domain tree nodes,scores, etc 
% domain_kdtree:    domain tree 
% 
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2

%[submodels,filters,model0] = squeeze_models(models_cell);
model0  = object_model.model;
times   = zeros(1,4);
tic;
[pyra]          = featpyramid(im,model0);

interval        = model0.interval;
levels_wt       = [[interval+1]:length(pyra.feat)];

%%----------------------------------------------------------------------
%% get unary terms & construct associated kd-trees
%%----------------------------------------------------------------------
doa = exist('fct_err','var'); 
if doa, 
    doa   = (~isempty(fct_err))&(~isinf(fct_err));
    if doa
        pyra = quantizepyramid(pyra,object_model.vocab,0);
    end
else
    fct_err = [];
end 
times(1)        = toc;

[part_trees,root_trees,domain_kdtree,bs,times] = unary_wrapper(object_model,levels_wt,pyra,doa,interval,times,fct_err);


%%----------------------------------------------------------------------
%% combine
%%----------------------------------------------------------------------
nparts  = 8;
%% in general {nmax, thr} will be scalars, so the code below will amount to
%%  [i,v,tr]  = mex_traverse(root_trees,part_trees,nparts,domain_kdtree,thr(1),nmax(1));
%% but if we want to generate plots by gathering timings for multiple values of
%% thr/nmax, we send these in as vectors and use the for-for loop below
cn = 0;
for id = 1:length(nmax)
    for k = 1:length(thr)
        tic;
        if doa,
            [i,v,tr]  = mex_traverse(root_trees,part_trees,nparts,domain_kdtree,thr(k),nmax(id),1,pyra.featp,pyra.padx+1,pyra.pady+1,object_model.w_root,object_model.w_part,bs);
        else
            [i,v,tr]  = mex_traverse(root_trees,part_trees,nparts,domain_kdtree,thr(k),nmax(id));
        end
        cn = cn + 1;
        times(3+cn) = toc;
    end
end

if nargout>1,
    scales      = pyra.scales(interval+1:end);
    submodels   = object_model.submodels; 
    nobjs       = length(submodels)/6;
    [dets,boxes,is] = sol2boxes(i,v,submodels,scales,nobjs,root_trees,part_trees);
end
