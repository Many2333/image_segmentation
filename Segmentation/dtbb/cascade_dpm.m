function [times,dCSC,bCSC] = cascade_dpm(im,object_model,thrs)

tic;
[pyra]          = featpyramid(im, object_model);
times(1)        = toc;

if length(thrs)==1
    object_model.cascade = object_model.cascadec{find(object_model.thrs ==thrs)};
    tin = tic;
    [dCSC, bCSC] = cascade_detect(pyra, object_model, thrs);
    t = toc(tin);
    [srt,idx]    = sort(dCSC(:,end),'descend');
    dCSC         = dCSC(idx,:);
    bCSC         = bCSC(idx,:);
    times = [times,t];
    return
end

%% ugly code to get  results for multiple thresholds with single pyramid
for ind = 1:length(thrs),
    thr = thrs(ind);
    thr_ind              = find(object_model.thrs ==thr);
    object_model.cascade = object_model.cascadec{thr_ind};
    tic;
    [dCSC{ind}, bCSC{ind}] = cascade_detect(pyra, object_model, thrs);
    t = toc;
    times = [times,t];
end
