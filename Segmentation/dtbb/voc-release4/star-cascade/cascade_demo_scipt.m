%function cascade_demo()
% Run cascade demo.
% 
% Note that unless you have compiled fconv.cc as your convolution
% function, you will be comparing a multi-threaded version of the
% DP algorithm to a single-threaded version of the cascade algorithm.

%load('VOC2007/car_final');
%impath = '000034.jpg';
%test(, model);
%fprintf('\nPress any key to continue with demo'); pause; fprintf('...ok\n\n');

%load('INRIA/inriaperson_final');
%impath = '000061.jpg';
%5test('000061.jpg', model);
%fprintf('\nPress any key to continue with demo'); pause; fprintf('...ok\n\n');

load('VOC2009/bicycle_final');
impath = '000084.jpg';
im = imread('000084.jpg');
%test('000084.jpg', model);
%function test(impath, model);

name = model.class;

clf;
fprintf('///// Running demo for %s /////\n\n', name);

fprintf('Loading a test image');
fprintf('...done\n');

%im = imread(impath);
subplot(1,3,1);
imagesc(im);
axis image;
axis off;

fprintf('Compute cascade thresholds');
fprintf('...done\n');

thresh = -.5;
pca = 5;
orig_model = model;
csc_model = cascade_model(model, '2009', pca, thresh);
orig_model.thresh = csc_model.thresh;

fprintf('Building the feature pyramid...');

th = tic();
pyra = featpyramid(double(im), csc_model);
tF = toc(th);

if 0,
    fprintf('done\n');
    fprintf('  --> Feature pyramid generation took %f seconds\n', tF);
    fprintf('Computing detections with dynamic programming...');
    th = tic;
    [dDP, bDP] = gdetect(pyra, orig_model, orig_model.thresh);
    tDP = toc(th);
    fprintf('done\n');
    fprintf('  --> DP detection took %f seconds\n', tDP);
end
fprintf('Computing detections with star-cascade...');


th = tic;
[dCSC, bCSC] = cascade_detect(pyra, csc_model, csc_model.thresh);
tCSC = toc(th);


fprintf('done\n');
fprintf('  --> Cascade detection took %f seconds\n', tCSC);
fprintf('  --> Speedup = %fx\n', tDP/tCSC);

b = getboxes(orig_model, im, dDP, reduceboxes(orig_model, bDP));
subplot(1,3,2);
showboxes(im, b);
title('dynamic programming detections');

b = getboxes(csc_model, im, dCSC, bCSC);
subplot(1,3,3);
showboxes(im, b);
title('star-cascade detections');
