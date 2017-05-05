%image = imread('..\VOCdevkit\VOC2012\JPEGImages\2007_000836.jpg');
image = imread('bee.jpg');

currentFolder = pwd;
addpath(genpath(currentFolder))

[labels, label_num] = getSLIC(image, 500, 20);
imagesc(labels);
[labels_x, labels_y] = size(labels);
[img_x, img_y, img_rgb] = size(image);
cnt = img_x * img_y;

Lab_image = RGB2Lab(image);
X = zeros(8, label_num);
label_pixel_num = zeros(1, label_num);

for i = 1 : labels_x
    for j = 1: labels_y
        num = labels(i, j) + 1;
        rgb = image(i, j, :);
        Lab = Lab_image(i, j, :);
        %[L, a, b] = RGB2Lab(rgb(:, :, 1), rgb(:, :, 2), rgb(:, :, 3));

        label_pixel_num(1, num) = label_pixel_num(1, num) + 1;
        X(1, num) = X(1, num) + double(i);
        X(2, num) = X(2, num) + double(j);
        X(3, num) = X(3, num) + double(rgb(:, :, 1));
        X(4, num) = X(4, num) + double(rgb(:, :, 2));
        X(5, num) = X(5, num) + double(rgb(:, :, 3));
        X(6, num) = X(6, num) + double(Lab(:, :, 1));
        X(7, num) = X(7, num) + double(Lab(:, :, 2));
        X(8, num) = X(8, num) + double(Lab(:, :, 3));
    end
end

for i = 1 : 8
    for j = 1 : label_num
        var1 = X(i, j);
        var2 = label_pixel_num(1, j);
        var3 = X(i, j) / label_pixel_num(1, j);
        X(i, j) = X(i, j) / label_pixel_num(1, j);
    end
end

[box_x_1, box_x_2, box_y_1, box_y_2] = getBox(img_x, img_y, 0.8);
disp([box_x_1, box_x_2, box_y_1, box_y_2]);

% A = zeros(8, label_num);
% 
% for i = 1 : label_num
%     if X(1, i) <= box_x_1 || X(1, i) >= box_x_2 || X(2, i) <= box_y_1 || X(2, i) >= box_y_2;
%         A(:, i) = X(:, i);
%     end
% end
% 
% %disp(A);
% 
% % noisy observations  
% sigma = 0.005;  
% e = sigma*randn(8,1);  
% 
% output = zeros(img_x, img_y);
% 
% for i = 1 : label_num
%     y = A(:, i);
%     if (abs(y) == 0)
%         y(1, 1) = 1;
%     end
%     % y = zeros(8, 1); 
%     % y(1, 1) = 1;
%     %disp(y);
%     %disp(i);
% 
%     % initial guess = min energy  
%     x0 = A'*y;  
% 
%     % take epsilon a little bigger than sigma*sqrt(K)  
%     epsilon =  sigma*sqrt(8)*sqrt(1 + 2*sqrt(2)/sqrt(8));  
% 
%     tic  
%     xp = l1qc_logbarrier(x0, A, [], y, epsilon, 1e-3);  
%     toc 
% 
%     tmp = norm((A * xp - y));
%     %disp(norm(xp, 1));
%     for j = 1 : img_x
%         for k = 1 : img_y
%             if labels(j, k) == i;
%                 output(j, k) = norm((A * xp - y));
%                 %output(j, k) = norm(xp);
%             end
%         end
%     end
% end
% 
% imagesc(output);
%disp(output);