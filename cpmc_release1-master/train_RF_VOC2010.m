function train_RF_VOC2010( exp_dir, rgb_folder, seg_folder, img_set, diversify_const, segm_pars )
%train RF regression model with image set specified by img_set
% Inputs:
%   - exp_dir = the folder where data (ranker models, codebooks, etc.) is
%   - rgb_folder = the folder where RGB images are
%   - seg_folder = the folder where SegmentationObject are
%   - img_set = full path of txt file
%   - diversify_const = diversification parameter ([0,1])
%   - segm_pars = struct with default values
%
% Outputs:
%   - model = model obtained

addpath('./code/');   
addpath('./external_code/');
addpath('./external_code/paraFmex/');
addpath('./external_code/imrender/vgg/');
addpath('./external_code/immerge/');
addpath('./external_code/color_sift/');
addpath('./external_code/vlfeats/toolbox/kmeans/');
addpath('./external_code/vlfeats/toolbox/kmeans/');
addpath('./external_code/vlfeats/toolbox/mex/mexa64/');
addpath('./external_code/vlfeats/toolbox/mex/mexglx/');
addpath('./external_code/globalPb/lib/');
addpath('./external_code/mpi-chi2-v1_5/');  

try
    matlabpool('open', 2);
catch
    return;
end

DefaultVal('*diversify_const', '0.75');
DefaultVal('*segm_pars', '[]');

if(isempty(segm_pars))
    segm_pars.pb_folder = [exp_dir './PB/'];
    segm_pars.name = 'dummy_masks';
    segm_pars.segm_methods = {'UniformSegmenter', 'LongRangeSegmenter', 'GridOfFramesSegmenter'};
    segm_pars.max_n_segms = [inf inf inf]; 
    segm_pars.min_n_pixels = 200; % 1000
    segm_pars.sigma = {1, 2, 0.8};
    segm_pars.resize_factor= 0.5;
    segm_pars.morph_open = true;
    segm_pars.filter_segments = {true, true, true};
    segm_pars.grid_dims = {[6 6], [6 6], [1 1]}; 
    % segm_pars.grid_dims = {[5 5], [5 5], [1 1]}; 
    segm_pars.window_gen_parms.kind = 'grid_sampler';        
    segm_pars.windows_folder = [exp_dir 'WindowsOfInterest/grid_sampler'];
    segm_pars.window_gen_parms.det_classes = [];
    segm_pars.randomize_N = 1000;
end

if(strcmp(img_set, 'train'))
    %load('list.mat');
    %list = part;
    list = textread('/media/baijm/Program/VOCdevkit/VOC2010/ImageSets/Segmentation/train.txt', '%s');
elseif(strcmp(img_set, 'trainval'))
    %load('list.mat');
    %list = part;
    %list = textread('/media/baijm/Program/VOCdevkit/VOC2010/ImageSets/Segmentation/trainval.txt', '%s');
end

%list = {'2007_000121';'2007_000256';'2007_001225';'2007_003580';'2007_003910';'2007_004481'; ...
%    '2007_004988';'2007_005210';'2007_006661';'2007_007098';'2007_007523';'2007_007726'; ...
%    '2007_007890';'2007_008403';'2007_008821';'2007_009779';'2008_000144';'2008_000187'; ...
%    '2008_000217';'2008_001761';'2008_002080';'2008_002182';'2008_002248';'2008_002719'; ...
%    '2008_003703';'2008_004080';'2008_006434';'2008_007355';'2008_008525';'2009_000662'; ...
%    '2009_001100';'2009_001735';'2009_002281';'2009_002472';'2009_003147';'2009_004409'; ...
%    '2010_002573';'2010_003170';'2010_003958';'2010_004493';'2010_004938';'2010_005198'; ...
%    '2010_005796';'2010_005932'};


D_sum = {[], []};

feat_mat = [exp_dir 'feat_sum.mat'];
ranker_mat = [exp_dir 'MySegmentRankers/' 'RF_model_fewfeats_2010_train.mat'];
list_mat = [exp_dir img_set '.mat'];
X_mat = [exp_dir 'X.mat'];
Y_mat = [exp_dir 'overlap_sum.mat'];
ranker = load(ranker_mat);

save(list_mat, 'list');

if(~exist(feat_mat, 'file'))
    % save feat_mat
    for i = 1:length(list)
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% 1. compute masks %%%%%%%%%%%%    
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
        fprintf('now processing %s,\t%d/%d\n', char(list(i)), i, length(list));
        t = tic();
        fprintf('\tStarting computation of figure-ground segmentations\n');
        %extract initial pool
        dir_name = [exp_dir 'MySegment6Mat/' segm_pars.name];
        if(~exist(dir_name, 'dir'))
            mkdir(dir_name);
        end
        file_name = [dir_name '/' char(list(i)) '.mat'];
        if(~exist(file_name, 'file'))
            [masks] = SvmSegm_extract_segments_nosave(rgb_folder, list(i), segm_pars);
            masks = masks{1};
            save(file_name, 'masks'); % save masks
        else
            var = load(file_name);
            masks = var.masks;
        end
        time_segm = toc(t);
        fprintf('\tTime computing figure-ground segmentations: %f\n', time_segm);
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%% 2. compute segment ranking features %%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        t = tic();
        fprintf('\tStarting feature extraction.\n');
        dir_name = [exp_dir 'MySegment6Features/' segm_pars.name];
        if(~exist(dir_name, 'dir'))
            mkdir(dir_name);
        end
        file_name = [dir_name '/' char(list(i)) '.mat'];
        if(~exist(file_name, 'file'))
            D = myget_features(exp_dir, char(list(i)), segm_pars.name, masks, ...
                ranker.segment_measurements, ...
                rgb_folder);     
            save(file_name, 'D');
        else
            var = load(file_name);
            D = var.D;
        end
        D_sum{1} = [D_sum{1}, D{1}];
        D_sum{2} = [D_sum{2}, D{2}];
        count(i) = size(D{1},2);    
        
        t_feats = toc(t);
        fprintf('\tTime getting all features: %f\n', t_feats); 
    end
    save(feat_mat, 'D_sum', 'count');
end

feat = load(feat_mat);

if(~exist(X_mat, 'file'))
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%% 3. normalize the features %%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
    [~, J1] = find(isnan(feat.D_sum{1}));
    [~, J2] = find(isnan(feat.D_sum{2}));
    J1 = unique(J1);
    J2 = unique(J2);
    J2 = setdiff(J2, J1);
    D_sum = feat.D_sum;
    if(~isempty(J1))
        for s = 1:numel(D_sum)
            D_sum{s}(:, J1) = [];
        end        
    end
    if(~isempty(J2))
        for s = 1:numel(D_sum)
            D_sum{s}(:, J2) = [];
        end
    end
    D_sum = cellfun(@double, D_sum, 'UniformOutput', false);
    D_sum = D_sum(:);
    if(iscell(ranker.scaling)) % --- scaling in ranker ---
        for j=1:numel(ranker.scaling)
            % --- scaling_type in ranker ---
            D_sum{j} =  scale_data(D_sum{j}, ranker.scaling_type, ranker.scaling{j});
        end
        D_scaled = cell2mat(D_sum);
    else
        D_scaled = cell2mat(D_sum);
        % --- scaling_type & scaling in ranker ---
        D_scaled = scale_data(D_scaled, ranker.scaling_type, ranker.scaling);
    end    
    
    save(X_mat, 'D_scaled');    
end

X = load(X_mat);
X = (X.D_scaled)';
    
if(~exist(Y_mat, 'file'))
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%% 4. compute mask quality %%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
    Y_sum = [];
    for k = 1:length(list)
        fprintf('computing overlap with ground truth : %s, %d/%d\n', char(list(k)), k, length(list));
        mask_mat = [exp_dir 'MySegment6Mat/' segm_pars.name '/' char(list(k)) '.mat'];
        allmask = load(mask_mat);
        Q = SvmSegm_segment_quality(char(list(k)), exp_dir, allmask.masks, 'overlap');
        Y = [];
        for p = 1:length(Q)
            Y(:, p) = Q(p).q;
        end
        Y = max(Y, [], 2);
        Y_sum = [Y_sum; Y];
    end   
    save(Y_mat, 'Y_sum');    
end

Y = load(Y_mat);
Y = Y.Y_sum;
[~, j1] = find(isnan(feat.D_sum{1}));
[~, j2] = find(isnan(feat.D_sum{2}));
j1 = unique(j1);
j2 = unique(j2);
j2 = setdiff(j2, j1);
if(~isempty(j1))
    Y(j1) = [];
end
if(~isempty(j2))
    Y(j2) = [];
end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%% 5. train RF model %%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%     
    %Y = max_overlap(Q); 
    %X = D_scaled;
    %model = classRF_train(X,Y,ntrees,mtry)    
end


function D = myget_features(exp_dir, img_name, mask_type, masks, feat_types, rgb_folder)
    segm_feats = {'SimpleSegmentFeatures', 'GestaltSegmentFeatures'};
    sf_in = [];
    
    t1 = tic();
    for i=1:numel(segm_feats)
        sf_in = [sf_in find(strcmp(segm_feats{i}, feat_types))];
    end    
    %sf_in = [1 2]
    
    % simple & gestalt
    D_1 = mySvmSegm_compute_segm_feats_nofiles(exp_dir, img_name, masks, ...
        segm_feats(sf_in), rgb_folder);    
    t_segm_feats1 = toc(t1);
    fprintf('\tTime getting first set of features: %f\n', t_segm_feats1);
    
    recog_feats_in = setdiff(1:numel(feat_types), sf_in);
    delete_local_feats = false;
    if(~isempty(recog_feats_in))
        t2 = tic();        
        % other features
        D_2 = SvmSegm_extract_all_class_features_nofiles(exp_dir, img_name, mask_type, ...
            feat_types(recog_feats_in),...
            delete_local_feats);         
        t_segm_feats2 = toc(t2);
        fprintf('\tTime getting second set of features: %f\n', t_segm_feats2);
        D = [D_1 D_2];        
    else
        D = D_1;
    end
    
    % reorder
    D = D([sf_in recog_feats_in]);%not reordered given the model
end
