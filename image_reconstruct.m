function image_reconstruct()
    addpath('cpmc-master');
    
    delete(gcp('nocreate'));
    parpool('local', 4);

    exp_dir = './BSDS300/';
    mat_dir = './mat/BSDS/';
    dump_dir = './output/BSDS/';
    img_name = '37073';
%     exp_dir = './VOCdevkit/VOC2012/';
%     img_name = '2007_002266';
    if exist([mat_dir, img_name, '.mat'], 'file') == 0
        disp('compute reconstruct image');
        img_reconstruct = segment_reconstruct(exp_dir, img_name);
        save([mat_dir, img_name], 'img_reconstruct');
    else
        disp('load mat');
        load([mat_dir, img_name]);
        figure('Name', 'reconstruct img'),
        imagesc(img_reconstruct);
    end
    
    img_binary = region_growing(img_reconstruct);
    imwrite(img_binary, [dump_dir, img_name], 'jpg');
    
    delete(gcp('nocreate'));