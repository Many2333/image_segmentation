function cpmc_example()  
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
    
    % create multiple threads (set how many you have)
    
     delete(gcp('nocreate'))
     parpool('local', 2);
%     N_THREADS = 2;
%     if(matlabpool('size')~=N_THREADS)
%         matlabpool('open', N_THREADS);
%     end

    exp_dir = './data/';
    %img_name = ['2010_000238'; ...% airplane and people   
    %    '2007_009084'; ...% dogs, motorbike, chairs, people    
    %    '2010_002868'; ...% buses   
    %    '2010_003781']; % cat, bottle, potted plants
    %img_name = ['Y0207_4';'Y0208_0';'Y0209_2';'Y0210_0';'Y0211_1';...
    %    'Y0214_0';'Y0304_1';'Y0307_0';'Y0315_2';'Y0403_5';...
    %    'Y0405_4';'Y0413_0';'Y0418_3';'Y0506_3';'Y0508_0'];
    %img_name = ['SJ0_0';'SJ0_1';'SJ0_2';'SJ0_3';'SJ0_4';'SJ0_5';'SJ0_6'];
    img_name=['SJ21_0';'SJ21_1';'SJ21_2';'SJ21_3';'SJ21_4';'SJ21_5';'SJ21_6';...
        'SJ22_0';'SJ22_1';'SJ22_2';'SJ22_3';'SJ22_4';'SJ22_5';'SJ22_6';...
        'SJ23_0';'SJ23_1';'SJ23_2';'SJ23_3';'SJ23_4';'SJ23_5';'SJ23_6';...
        'SJ24_0';'SJ24_1';'SJ24_2';'SJ24_3';'SJ24_4';'SJ24_5';...
        'SJ31_0';'SJ31_1';'SJ31_2';'SJ31_3';'SJ31_4';...
        'SJ32_0';'SJ32_1';'SJ32_2';'SJ32_3';'SJ32_4';...
        'SJ33_0';'SJ33_1';'SJ33_2';'SJ33_3';'SJ33_4';...
        'SJ34_0';'SJ34_1';'SJ34_2';'SJ34_3';'SJ34_4';...
        'SJ41_0';'SJ41_1';'SJ41_2';'SJ41_3';'SJ41_4';'SJ41_5';...
        'SJ51_0';'SJ51_1';'SJ51_2';'SJ51_3';'SJ51_4';'SJ51_5';...
        'SJ52_0';'SJ52_1';'SJ52_2';'SJ52_3';'SJ52_4';'SJ52_5';'SJ52_6';...
        'SJ53_0';'SJ53_1';'SJ53_2';'SJ53_3';'SJ53_4';'SJ53_5';'SJ53_6';...
        'SJ54_0';'SJ54_1';'SJ54_2';'SJ54_3';'SJ54_4';'SJ54_5';'SJ54_6'];
        
    for i=1:length(img_name)
        disp(img_name(i,:));
        [masks, scores] = cpmc(exp_dir, img_name(i,:));
        I = imread([exp_dir '/JPEGImages/' img_name(i,:) '.jpg']);
        % visualization and ground truth score for whole pool
        fprintf(['Best segments from initial pool of ' int2str(size(masks,3))]);
        Q = SvmSegm_segment_quality(img_name(i,:), exp_dir, masks, 'overlap');
        if(isempty(Q))
            continue;
        end
        %save('duh_32.mat', 'Q');
        save([img_name(i,:) '_quality.mat'],'Q');
        avg_best_overlap = mean(max([Q.q]))
        disp(img_name(i,:));
        %SvmSegm_show_best_segments(I,Q,masks);
        save_segments(masks, scores, I, img_name(i,:), exp_dir);
        
        % visualization and ground truth score for top 200 segments    
        try
        top_masks = masks(:,:,1:200);
        catch
            continue;
        end
        figure;
        disp('Best 200 segments after filtering');
        Q = SvmSegm_segment_quality(img_name(i,:), exp_dir, top_masks, 'overlap');
        avg_best_overlap = mean(max([Q.q]))
        disp(img_name(i,:));
        SvmSegm_show_best_segments(I,Q,top_masks);
        fprintf('Best among top 200 after filtering\n\n');        
    end
       
end
