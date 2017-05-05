function D = mySvmSegm_compute_segm_feats_nofiles( exp_dir, img_name, masks, feat_type , rgb_folder)
%compute segment features (JPG folder is changeable)
% Inputs:
%   - exp_dir = where PB is
%   - rgb_folder = path of RGB images, with '/'
%   - img_name = image name without extension
%   - masks = masks got by cpmc_masks
%   - feat_type = indices of feature types
% Outputs:
%   - D = unnormalized feature

assert(iscell(feat_type));

I = imread([rgb_folder img_name '.jpg']);
parfor h=1:numel(feat_type)
    if(strcmp(feat_type{h}, 'SimpleSegmentFeatures'))
        % cut*8, region shape location * 19
        f = SimpleSegmentFeatures(I, masks, [exp_dir 'PB/' img_name '_PB']);
    elseif(strcmp(feat_type{h}, 'GestaltSegmentFeatures'))
        % texton inter/intra*2, brightness inter/intra*2, contour
        % inter/intra*2, continuity
        f =GestaltSegmentFeatures(I, masks, [exp_dir 'PB/' img_name '_PB']);
    else
        error('no such feature type');
    end
    
    f = f.compute_all_feats();
        
    D{h} = f.get_feats();
end    

end

