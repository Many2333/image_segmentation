function [times_c,dets,boxes] = cascade_lup(im,object_model,thrs,f_errs)
tic;
pyra            = featpyramid(im, object_model.model);
pyra            = quantizepyramid(pyra,object_model.vocab,0,0);
times(1)        = toc;
thr_cascade     = object_model.thrsc;

if (length(f_errs)*(length(thrs)))==1
    t_ind       = find(object_model.thresh ==thrs);
    thrs_in     = thr_cascade{t_ind};
    [times_csc,dets,boxes] = imgdetect_csc_lu(object_model,thrs_in,f_errs,pyra);
    times_c = [times,times_csc];
    return
end

%% some ugly code for the case when multiple thresholds and error factors are used
%% (in that case we avoid calling the pyramid code multiple times, and manage to get
%% our plots before the deadline)
for f_ind = 1:length(f_errs)
    times_c{f_ind} = times(1);
    fct_err = f_errs(f_ind);
    for ind = 1:length(thrs),
        thr   = thrs(ind);
        t_ind = find(object_model.thresh ==thr);
        thrs_in  = thr_cascade{t_ind};
        [dets,boxes,times_csc] = imgdetect_csc_lu(object_model,thrs_in,fct_err,pyra);
        dets{f_ind,t_ind}  = d;
        boxes{f_ind,t_ind} = b;
        times_c{f_ind} = [times_c{f_ind},times_csc];
    end
end
