function [dets,boxes,is] = sol2boxes(i,v,submodels,scales,nobjs,root_trees,part_trees,nflip)
%% turn the outputs of branch and bound into original DPM format
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2

object_ids  = [submodels(i(:,end-1)).md];
objs        = unique(object_ids);
if nargin<nargin(mfilename),
    nflip = 2;
end
dets  = cell(1,nobjs);
boxes = cell(1,nobjs);
is    = cell(1,nobjs);
scls    = single(1./scales);   % box scaling factor
dets = [];
boxes = [];
for k=1:length(objs),   %% loop over object categories
    obj_id      = objs(k);
    %% find i's for current object categroy
    idswt       = find(object_ids==obj_id);
    iwt         = i(idswt,:);    
    vwt         = v(idswt);
    [srt,idx]   = sort(vwt,'descend');
    [detsn, boxesn] = turn_to_detections(iwt(idx,:),vwt(idx),scls,submodels,root_trees,part_trees,nflip);
    
    is{obj_id} = iwt(idx,:);
    dets = [dets;detsn];
    boxes = [boxes;boxesn];
end

function [dets, boxes] = turn_to_detections(i,v,scls,submodels,root_trees,part_trees,nflip)
% turns BB detections into  boxes similar to the ones returned by the original DPM code

nsol = length(v);
nmax = submodels(1).nmax;
dets = zeros(nsol,6);
if nsol>1e3
    disp('something must be wrong');
    dets  = [];
    boxes = [];
    keep  = 1:nsol;
    return;
end

for sl = [1:nsol],
    scl         = i(sl,end-2);
    mxt         = mod(i(sl,end-1)-1,6)+1;
    idtree      = i(sl,end-1);
    prt_ids     = i(sl,2:end-3);
    sclp        = 8*scls(scl);
    for prt = 1:length(prt_ids),
        pid = prt_ids(prt);
        if prt==1,
            center                 = root_trees{scl}.centers(:,pid);
            locations_parts(:,prt) = sclp*center;
            offset = 0;
        else
            center =  part_trees{scl}.centers(:,pid);
            locations_parts(:,prt)   = sclp*center; %(messages{prt}.centers(:,pid)) % - relations(prt).offset');
            if prt==2,
                offset = (ceil(mxt/2)-1)*(length(prt_ids)-1)*4*nflip;
                if iseven(mxt),
                    offset = offset + 4;
                end
            else
                offset = offset + 4*(nflip==2);
            end
        end
        lc  = locations_parts(:,prt);
        if prt==1,
            sb = submodels(idtree).sw;
            bx  = [lc(1)+1,lc(2)+1,lc(1) + sb(2)*sclp,   lc(2) + sb(1)*sclp];
            dets(sl,:) = [bx,mxt,v(sl)];
        else
            bx =  [lc(1)+1,lc(2)+1,lc(1) + 3*sclp,       lc(2) + 3*sclp];
        end
        idxs =  offset + submodels(idtree).idx_box{prt};
        boxes(sl,idxs) = bx;
    end
    boxes(sl,nmax+1) = mxt;
    boxes(sl,nmax+2) = v(sl);
end
%ds          = dist(i(:,[2,11,12],i(:,[2,11,12])'));
%mtrx = triu(ds,1) + (triu(ds,1)==0);


