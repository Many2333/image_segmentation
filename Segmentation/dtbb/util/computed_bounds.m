function [im,node_map]= computed_bounds(mxt_best,scl_best,gr,tr,offsets,sz,lwt)

if isempty(mxt_best),
    %% mixture and scale indexes for the 1st-best detection
    mxt_best    = tr(4,end);
    scl_best    = tr(3,end);
end

parents = tr(1,:);
scales  = tr(3,:);
mxts    = tr(4,:);

child_1 = tr(5,:);
child_2 = tr(6,:);

cost_1 = tr(7,:);
cost_2 = tr(8,:);

wt      = find((mxts==mxt_best)&(scl_best==scales));
leaf    = find(sum(gr.ranges,1)==0); 

offv = offsets(1);
offh = offsets(2);
pd = [offh-1;offv-1];

im      = 10*ones(max(gr.centers(2,leaf))+offv,max(gr.centers(1,leaf))+offh);
[sv,sh] = size(im);

if 0
levels      = tree_levels(gr);
if exist('lwt','var')
    levs        = (levels==lwt);
    in_level    = find(levs(child_1(wt)));
    wt          = wt(in_level);
    node_map    = ones(size(im));
end
end

    
for k=1:length(wt),
    idx = wt(k);
    [range_v,range_h,range] = get_bbox(gr,child_1(idx),pd);
   
    %% by default the code uses the costs of the children intervals 
    %% but if the children is identical to the parent, it means we heat 
    %% a leaf node in the domain kd-tree, and kept repeating the bound computation
    %% at the same leaf, until the source intervals became also leaves
    %% (earlier they were intervals, resulting in a discrepancy between the 
    %% lower and upper bounds)
    
    if child_1(idx)==parents(idx),
        cost_1(idx) = tr(2,idx);
    end
    if child_2(idx)==parents(idx),
        cost_2(idx) = tr(2,idx);
    end
    
    range_v = min(max(range_v,1),sv);
    range_h = min(max(range_h,1),sh);
    
    im(range_v,range_h) = min(im(range_v,range_h),cost_1(idx));
    
    node_map(range_v,range_h) = idx;
    
    if (child_2(idx)>0)
        [range_v,range_h,range]   = get_bbox(gr,child_2(idx),pd);
       
        range_v = min(max(range_v,1),sv);
        range_h = min(max(range_h,1),sh);
        im(range_v,range_h) = min(im(range_v,range_h),cost_2(idx));
    end
end
im = im(1:min(sz(1),end),1:min(sz(2),end));


function [range_v,range_h,range] = get_bbox(gr,idx,pd);
cen_cur = gr.centers(:,idx)+1 + pd;
range   = gr.ranges(:, idx);

range_h = [floor(cen_cur(1) - range(1)):ceil(cen_cur(1) + range(1))];
range_v = [floor(cen_cur(2) - range(2)):ceil(cen_cur(2) + range(2))];
