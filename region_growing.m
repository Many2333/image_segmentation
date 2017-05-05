function img_output = region_growing(img_input)
    [height, width] = size(img_input);
    img_output = zeros(height, width);
    log = zeros(height, width);
    
    [max_x, max_y] = find(img_input == max(max(img_input)));
    for i = 1 : size(max_x)
        x = max_x(i);
        y = max_y(i);
        img_output(x, y) = 1;
    end
    change = 1;
    threshold = 25;
    threshold_rate = 0.5;
    min_limit = 60;
    
    while change ~= 0
        %disp(change);
        change = 0;
        
        [x, y] = find(img_output == 1);
        for i = 1 : size(x);
            cur_x = x(i);
            cur_y = y(i);
            
            if log(cur_x, cur_y) == 1
                continue;
            end
            
            log(cur_x, cur_y) = 1;
            
            if cur_x - 1 >= 1
                i1 = cur_x - 1;
                if abs(img_input(i1, cur_y) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, cur_y) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, cur_y) ~= 1 && img_input(i1, cur_y) >= min_limit
                    img_output(i1, cur_y) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= height
                i1 = cur_x + 1;
                if abs(img_input(i1, cur_y) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, cur_y) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, cur_y) ~= 1 && img_input(i1, cur_y) >= min_limit
                    img_output(i1, cur_y) = 1;
                    change = change + 1;
                end
            end
            if cur_y - 1 >= 1
                j1 = cur_y - 1;
                if abs(img_input(cur_x, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(cur_x, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(cur_x, j1) ~= 1 && img_input(cur_x, j1) >= min_limit
                    img_output(cur_x, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_y + 1 <= width
                j1 = cur_y + 1;
                if abs(img_input(cur_x, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(cur_x, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(cur_x, j1) ~= 1 && img_input(cur_x, j1) >= min_limit
                    img_output(cur_x, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x - 1 >= 1 && cur_y - 1 >= 1
                i1 = cur_x - 1;
                j1 = cur_y - 1;
                if abs(img_input(i1, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, j1) ~= 1 && img_input(i1, j1) >= min_limit
                    img_output(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x - 1 >= 1 && cur_y + 1 <= width
                i1 = cur_x - 1;
                j1 = cur_y + 1;
                if abs(img_input(i1, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, j1) ~= 1 && img_input(i1, j1) >= min_limit
                    img_output(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= height && cur_y - 1 >= 1
                i1 = cur_x + 1;
                j1 = cur_y - 1;
                if abs(img_input(i1, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, j1) ~= 1 && img_input(i1, j1) >= min_limit
                    img_output(i1, j1) = 1;
                    change = change + 1;
                end
            end
            if cur_x + 1 <= height && cur_y + 1 <= width
                i1 = cur_x + 1;
                j1 = cur_y + 1;
                if abs(img_input(i1, j1) / img_input(cur_x, cur_y)) >= threshold_rate && ...
                        abs(img_input(i1, j1) - img_input(cur_x, cur_y)) <= threshold && ...
                        img_output(i1, j1) ~= 1 && img_input(i1, j1) >= min_limit
                    img_output(i1, j1) = 1;
                    change = change + 1;
                end
            end
        end
    end

    figure('Name', 'binary_img'),
    imagesc(img_output);
    
end

