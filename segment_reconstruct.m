function img_reconstruct = segment_reconstruct(exp_dir, img_name)
    addpath('./cpmc-master');
    addpath('./cpmc-master/code/');    
    addpath('./cpmc-master/external_code/');
    addpath('./cpmc-master/external_code/paraFmex/');
    addpath('./cpmc-master/external_code/imrender/vgg/');
    addpath('./cpmc-master/external_code/immerge/');
    addpath('./cpmc-master/external_code/color_sift/');
    addpath('./cpmc-master/external_code/vlfeats/toolbox/kmeans/');
    addpath('./cpmc-master/external_code/vlfeats/toolbox/kmeans/');
    addpath('./cpmc-master/external_code/vlfeats/toolbox/mex/mexa64/');
    addpath('./cpmc-master/external_code/vlfeats/toolbox/mex/mexglx/');
    addpath('./cpmc-master/external_code/globalPb/lib/');
    addpath('./cpmc-master/external_code/mpi-chi2-v1_5/');
    addpath('./Segmentation/');
    addpath('./l1magic/Optimization/');
    addpath('./saliency');
    
    lambda_1 = 0.01;
    lambda_2 = 0.01;
    lambda_3 = 0.01;
    
    %start background reconstruct
    
    slic_source_path = [exp_dir, 'JPEGImages/', img_name, '.jpg'];
    slic_source_img = imread(slic_source_path);
    lab_img = RGB2Lab(slic_source_img);
    [img_width, img_height, img_rgb] = size(slic_source_img);
        
    [box_x_min, box_x_max, box_y_min, box_y_max] = getBoundingBox_2(slic_source_img);
    %[box_x_min, box_x_max, box_y_min, box_y_max] = getBoundingBox(img_name);
    %Saliency_CVPR2009(slic_source_img);
    
    [slic_labels, superpixels_num] = getSLIC(slic_source_img, 400, 10);
    fprintf('superpixel total num = %d\n', superpixels_num);
    
%     visualize slic segments
%     [sx,sy]=vl_grad(double(slic_labels), 'type', 'forward') ;
%     s = find(sx | sy) ;
%     size(find(sx | sy))
%     imp = slic_source_img ;
%     imp([s s+numel(imp(:,:,1)) s+2*numel(imp(:,:,1))]) = 0 ;
%     o = imp;
%     imshow(o); 
    
    bg_base = zeros(8, superpixels_num);
    label_pixels_num = zeros(1, superpixels_num);
    
    for x = 1 : img_width
        for y = 1 : img_height
            num = slic_labels(x, y) + 1;
            rgb = slic_source_img(x, y, :);
            lab = lab_img(x, y, :);
            
            label_pixels_num(1, num) = label_pixels_num(1, num) + 1;
            bg_base(1, num) = bg_base(1, num) + int32(x);
            bg_base(2, num) = bg_base(2, num) + int32(y);
            bg_base(3, num) = bg_base(3, num) + double(rgb(:, :, 1));
            bg_base(4, num) = bg_base(4, num) + double(rgb(:, :, 2));
            bg_base(5, num) = bg_base(5, num) + double(rgb(:, :, 3));
            bg_base(6, num) = bg_base(6, num) + double(lab(:, :, 1));
            bg_base(7, num) = bg_base(7, num) + double(lab(:, :, 2));
            bg_base(8, num) = bg_base(8, num) + double(lab(:, :, 3));
        end
    end
    
    for i = 1 : 8
        for j = 1 : superpixels_num
            if i == 1 || i == 2
                bg_base(i, j) = int32(bg_base(i, j) / label_pixels_num(1, j));
            else
                bg_base(i, j) = bg_base(i, j) / label_pixels_num(1, j);
            end
        end
    end
    
    A = bg_base;
    
    for i = 1 : superpixels_num
        if A(1, i) > box_x_min && A(1, i) < box_x_max && A(2, i) > box_y_min && A(2, i) < box_y_max
            A(:, i) = 0;
        end
    end
    
    bg_rec_vector = zeros(superpixels_num, superpixels_num);
    bg_reconstruct = zeros(img_width, img_height);
    epsilon = 0;
    
    disp('start solving p2 problem');
    for i = 1 : superpixels_num
        fprintf('current superpixel num = %d\n', i);
        y = bg_base(:, i);
        
        x0 = zeros(superpixels_num, 1);
       
%         lamda = 0.005 * sqrt(2 * log(double(superpixels_num)));
%         xp = BPDN_quadprog(y, A, lamda);
        if A(1, i) == 0
            xp = l1qc_logbarrier(x0, A, [], y, epsilon, 1e-3);
        else
            x0(i, 1) = 1;
            xp = x0;
        end
        bg_rec_vector(:, i) = xp;
        
        error = norm(A * xp - y) * 1000000;
        
        %disp(error);
        [x, y] = find(slic_labels == i - 1);
        
        for j = 1 : size(x)
            bg_reconstruct(x(j), y(j)) = error;
        end
        
    end
    
    figure('Name', 'background_reconstruct'),
    imagesc(bg_reconstruct); 
    
    %start segment reconstruct
    disp('start segment reconstruce');
    
    [masks, scores] = cpmc(exp_dir, img_name);
    
    top_num = 200;
    if size(masks, 3) < top_num
        top_masks = masks;
    else
        top_masks = masks(:, :, 1:top_num);
    end
    masks_num = size(top_masks, 3);
    
%     for i = 1 : 5
%         figure('Name', 'cpmc-ans');
%         imagesc(masks(:, :, i));
%     end
    
    for x = 1 : img_width
        for y = 1 : img_height
            if x < box_x_min || x > box_x_max
                top_masks(x, y, :) = 0;
            elseif y < box_y_min || y > box_y_max
                top_masks(x, y, :) = 0;
            end
        end
    end
    
    disp('start computing seg_base');
    seg_base = zeros(masks_num, superpixels_num);
    for i = 1 : masks_num
        for j = 1 : superpixels_num
            bg_vector = bg_base(:, j);
            mask = top_masks(:, :, i);
            x = int32(bg_vector(1, 1));
            y = int32(bg_vector(2, 1));
            val = mask(x, y);
            if x > 0 && y > 0 && mask(x, y) == 1
                seg_base(i, j) = 1;
            end
        end
    end
    
    disp('start computing segment reconstruct');
    epsilon = lambda_2 / 2 * lambda_3;
    M = zeros(masks_num, superpixels_num);
    W = zeros(masks_num, superpixels_num);
    seg_reconstruct = zeros(img_width, img_height);
    
    for i = 1 : superpixels_num
%         tmp_1 = bg_rec_vector(:, i);
%         tmp_3 = seg_base * tmp_1;
        
        W(:, i) = seg_base(:, i) - seg_base * bg_rec_vector(:, i);
    end
    
    for i = 1 : masks_num
        for j = 1 : superpixels_num
            if W(i, j) > epsilon
                M(i, j) = W(i, j) - epsilon;
            elseif W(i, j) < -epsilon
                M(i, j) = W(i, j) + epsilon;
            else
                M(i, j) = 0;
            end
        end
    end
    
    for i = 1 : superpixels_num
        mi = M(:, i);
        d = mi + seg_base * bg_rec_vector(:, i);
        
        [x, y] = find(slic_labels == i - 1);
        
        for j = 1 : size(x)
            seg_reconstruct(x(j), y(j)) = sum(d);
        end
    end
    
    figure('Name', 'seg_reconstruct'),
    imagesc(seg_reconstruct);
    
    figure('Name', 'final_reconstruct'),
    factor = max(max(seg_reconstruct)) / max(max(bg_reconstruct));
    factor = 10 ^ floor(log10(factor)) + 1;
    img_reconstruct = bg_reconstruct .* factor + seg_reconstruct;
    imagesc(img_reconstruct);
    
end