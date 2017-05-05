function preprocess(pic_name, pic_no, save, disp)
img_name = ['/' pic_name '/pic_' num2str(pic_no)];
origin_path = './origin';
mask_path = './mask';
segobj_path = './SegmentationObject';

if(~isdir(segobj_path))
    mkdir(segobj_path);
end

%read img (1920*1080)
try
    origin = imread([origin_path img_name '.jpg']);
catch
    disp([origin_path img_name '.jpg does not exist.']);
    return;
end
try
    mask = imread([mask_path img_name '.bmp']);
catch
    disp([mask_path img_name '.bmp does not exist.']);
    return;
end

%resize img to (500*...) or (...*500)
if(size(origin,1) >= size(origin,2))
    factor = 500.0/size(origin,1);
else
    factor = 500.0/size(origin,2);
end

origin_resized = imresize(origin, [floor(size(origin,1)*factor) floor(size(origin,2)*factor)]);
mask_resized = imresize(mask, [floor(size(mask,1)*factor) floor(size(mask,2)*factor)]);

%save origin as jpg, save segment as png
if(save)
    imwrite(origin_resized, [origin_path '/' pic_name '_' num2str(pic_no) '_small.jpg']);
    imwrite(mask_resized, [mask_path '/' pic_name '_' num2str(pic_no) '_small.bmp']);
    
    % color antenna area
    load('./colormap256.mat');
    segmentation = uint8(zeros(size(origin_resized,1), size(origin_resized,2)));
    %convert all nonzero value in mask to 21 (label of antenna)
    [I, J] = find(mask_resized);
    for i = 1: length(I)
        segmentation(I(i), J(i)) = 21;
    end
    imwrite(segmentation, map256, [segobj_path '/' pic_name '_' num2str(pic_no) '.png']);
end

%draw mask on origin & show
if(disp)
    bg = zeros(size(mask_resized,1), size(mask_resized,2), 3);
    for i = 1: length(I)
        bg(I(i), J(i), 1) = map256(21, 1);
        bg(I(i), J(i), 2) = map256(21, 2);
        bg(I(i), J(i), 3) = map256(21, 3);
        bg = uint8(255*bg);
    end
    res = immerge(origin_resized, bg, 0.5);
    imshow(res);
end

end