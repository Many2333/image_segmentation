function [box_x_min, box_x_max, box_y_min, box_y_max] = getBoundingBox_2(img)
    [origin_width, origin_height, rgb] = size(img); 
    max_side = 120;
    yu_value = 0.8;

    img_lab = pre_rgb2lab(img, max_side);
    %img_lab = RGB2Lab(img);
    [img_scale_1,img_scale_2, img_scale_3,img_scale_4] = get4Scale(img_lab);
    [DistanceValue_scale_1_t1, DistanceValue_scale_1_exp, DistanceValue_scale_1_t1_rang, DistanceValue_scale_1_exp_rang] = ...
        distanceValueMap_search_onescale_2(img_scale_1, max_side);
    [DistanceValue_scale_2_t1, DistanceValue_scale_2_exp, DistanceValue_scale_2_t1_rang, DistanceValue_scale_2_exp_rang] = ...
        distanceValueMap_search_onescale_2(img_scale_2, max_side);
    [DistanceValue_scale_3_t1, DistanceValue_scale_3_exp, DistanceValue_scale_3_t1_rang, DistanceValue_scale_3_exp_rang] = ...
        distanceValueMap_search_onescale_2(img_scale_3, max_side);
    [DistanceValue_scale_4_t1, DistanceValue_scale_4_exp, DistanceValue_scale_4_t1_rang, DistanceValue_scale_4_exp_rang] = ...
        distanceValueMap_search_onescale_2(img_scale_4, max_side);

    value_C_1 = Center_weight(DistanceValue_scale_1_exp_rang, yu_value);
    value_C_2 = Center_weight(DistanceValue_scale_2_exp_rang, yu_value);
    value_C_3 = Center_weight(DistanceValue_scale_3_exp_rang, yu_value);
    value_C_4 = Center_weight(DistanceValue_scale_4_exp_rang, yu_value);

    [h, w] = size(value_C_1);

    value_C_1_resize = value_C_1;
    value_C_2_resize = imresize(value_C_2, [h, w]);
    value_C_3_resize = imresize(value_C_3, [h, w]);
    value_C_4_resize = imresize(value_C_4, [h, w]);
    value_C_sum = (value_C_1_resize + value_C_2_resize + value_C_3_resize + value_C_4_resize) / 4;

%     figure('Name', 'sum'),
%     imshow(value_C_sum);
%     figure('Name', '1'),
%     imshow(value_C_1_resize);
%     figure('Name', '2'),
%     imshow(value_C_2_resize);
%     figure('Name', '3'),
%     imshow(value_C_3_resize);
%     figure('Name', '4'),
%     imshow(value_C_4_resize);
    
    input_saliency = value_C_1_resize;
    [max_x, max_y] = find(input_saliency == max(max(input_saliency)));
    main_object = zeros(h, w);
    main_object(max_x, max_y) = 1;
    change = 1;
    threshold = 0.1;
    min_limit = 0.1;
    
    while change ~= 0
%         disp(change);
        change = 0;
        
        [x, y] = find(main_object == 1);
        for i = 1 : size(x);
            cur_x = x(i);
            cur_y = y(i);
            if cur_x - 1 >= 1
                i1 = cur_x - 1;
                if abs(input_saliency(i1, cur_y) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, cur_y) ~= 1 && input_saliency(i1, cur_y) >= min_limit
                    main_object(i1, cur_y) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= h
                i1 = cur_x + 1;
                if abs(input_saliency(i1, cur_y) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, cur_y) ~= 1 && input_saliency(i1, cur_y) >= min_limit
                    input_saliency(i1, cur_y) = 1;
                    change = change + 1;
                end
            end
            if cur_y - 1 >= 1
                j1 = cur_y - 1;
                if abs(input_saliency(cur_x, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(cur_x, j1) ~= 1 && input_saliency(cur_x, j1) >= min_limit
                    main_object(cur_x, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_y + 1 <= w
                j1 = cur_y + 1;
                if abs(input_saliency(cur_x, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(cur_x, j1) ~= 1 && input_saliency(cur_x, j1) >= min_limit
                    main_object(cur_x, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x - 1 >= 1 && cur_y - 1 >= 1
                i1 = cur_x - 1;
                j1 = cur_y - 1;
                if abs(input_saliency(i1, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, j1) ~= 1 && input_saliency(i1, j1) >= min_limit
                    main_object(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x - 1 >= 1 && cur_y + 1 <= w
                i1 = cur_x - 1;
                j1 = cur_y + 1;
                if abs(input_saliency(i1, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, j1) ~= 1 && input_saliency(i1, j1) >= min_limit
                    main_object(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= h && cur_y - 1 >= 1
                i1 = cur_x + 1;
                j1 = cur_y - 1;
                if abs(input_saliency(i1, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, j1) ~= 1 && input_saliency(i1, j1) >= min_limit
                    main_object(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= h && cur_y + 1 <= w
                i1 = cur_x + 1;
                j1 = cur_y + 1;
                if abs(input_saliency(i1, j1) - input_saliency(cur_x, cur_y)) <= threshold && ...
                        main_object(i1, j1) ~= 1 && input_saliency(i1, j1) >= min_limit
                    main_object(i1, j1) = 1;
                    change = change + 1;
                end
            end
        end
    end
    
    figure;
    imagesc(main_object);

    [x, y] = find(main_object == 1);
    box_x_min = min(x);
    box_x_max = max(x);
    box_y_min = min(y);
    box_y_max = max(y);
    
    rate_x = h / origin_width;
    rate_y = w / origin_height;
    
    box_x_min = int32(box_x_min / rate_x);
    box_x_max = int32(box_x_max / rate_x);
    box_y_min = int32(box_y_min / rate_y);
    box_y_max = int32(box_y_max / rate_y);
end