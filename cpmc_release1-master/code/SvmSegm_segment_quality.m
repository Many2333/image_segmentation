% Copyright (C) 2010 Joao Carreira
%
% This code is part of the extended implementation of the paper:
% 
% J. Carreira, C. Sminchisescu, Constrained Parametric Min-Cuts for Automatic Object Segmentation, IEEE CVPR 2010
% 

function Q = SvmSegm_segment_quality(img_name, exp_dir, masks, segm_quality_type)             
    if iscell(masks)
        masks = cell2mat(masks);
    end

    %name = [exp_dir 'SegmentationObject/' img_name '.bmp'];
    name = [exp_dir 'SegmentationObject/' img_name '.png'];
    
    %return if mask does not exist
    %if(~exist(name,'file'))
    %    Q = [];
    %    return;
    %end
    
    %convert RGB BMP to grayscale
    %rgb_gt_seg = imread(name);
    %ground_truth_obj_segs = rgb2gray(rgb_gt_seg);
    ground_truth_obj_segs = imread(name);

    %label all antenna as 21
    %ground_truth_obj_segs(find(ground_truth_obj_segs)) = 21;
    un = unique(ground_truth_obj_segs);
    un(un==0) = [];
    un(un==255) = [];
    
    care = (ground_truth_obj_segs~=255);    
    parfor k=1:numel(un)
        ground_truth_k = zeros(size(ground_truth_obj_segs));
        ground_truth_k(ground_truth_obj_segs == un(k)) = 1;

        [duh1, duh2, this_Q] = myCalcCandScoreFigureGroundAll(masks,ground_truth_k, segm_quality_type, care);
        Q(k).q = this_Q;
    end
end
