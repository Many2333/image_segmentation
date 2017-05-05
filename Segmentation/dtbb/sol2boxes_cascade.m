function [dets,boxes,is] = sol2boxes_cascade(object_ids,v,submodels,scales,xall,yall,nflip)
%% turn the outputs of branch and bound into original DPM format
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2

%object_ids  = [submodels(i(:,end-1)).md];
objs        = unique(object_ids(1,:));
if nargin<nargin(mfilename)
    nflip = 2;
end
nobjs = length(objs);
dets  = cell(1,nobjs);
boxes = cell(1,nobjs);
is    = cell(1,nobjs);
scls    = single(1./scales);   % box scaling factor
dets = [];
boxes = [];
for k=1:length(objs),   %% loop over object categories
    obj_id      = objs(1,k);
    %% find i's for current object categroy
    idswt       = find(object_ids(1,:)==obj_id);
    iwt         = object_ids(2:end,idswt);
    vwt         = v(idswt);
    [srt,idx]   = sort(vwt,'descend');
    idxs        = idswt(idx);
    [detsn, boxesn] = turn_to_detections(iwt(:,idx)',vwt(idx),scls,submodels,xall(idxs,:),yall(idxs,:),nflip);
    dets = [dets;detsn];
    boxes = [boxes;boxesn];
    %is{obj_id} = iwt(idx,:);
end

function [dets, boxes] = turn_to_detections(i,v,scls,submodels,xall,yall,nflip)
% turns BB detections into  boxes similar to the ones returned by the original DPM code

nsol = length(v);
nmax = submodels(1).nmax;
for sl = [1:nsol],
    scl         = i(sl,1);
    mxt         = mod(i(sl,2)-1,6)+1;
    idtree      = i(sl,2);
    %prt_ids     = i(sl,2:end-3);
    sclp        = 8*scls(scl-10);
    nparts      = size(xall,2);
    for prt = 1:nparts
        if prt==1,
            center  = [xall(sl,prt);yall(sl,prt)];
            locations_parts(:,prt) = sclp*center;
            offset = 0;
        else
            center  = [xall(sl,prt);yall(sl,prt)];
            locations_parts(:,prt)   = sclp*center/2; %(messag
            if prt==2,
                offset = (ceil(mxt/2)-1)*(nparts-1)*4*nflip;
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

