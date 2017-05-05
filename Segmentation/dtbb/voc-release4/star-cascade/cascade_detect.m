function [dets, boxes, t] = cascade_detect(pyra, model, thresh)

tic;
% gather PCA root filters for convolution
numrootfilters = length(model.rootfilters);
rootfilters = cell(numrootfilters, 1);

for i = 1:numrootfilters
    rootfilters{i} = model.rootfilters{i}.wpca;
end
for k =1:length(model.rootfilters), rootfilters_0{k} = model.rootfilters{k}.w; end
projpyra = projectpyramid(model, pyra);
% else
%     for i = 1:numrootfilters
%         rootfilters{i} = model.rootfilters{i}.w;
%     end
% end
% compute PCA projection of the feature pyramid
% stage 0: convolution with PCA root filters is done densely
% before any pruning can be applied
numrootlocs = 0;
nlevels = size(pyra.feat,1);
rootscores = cell(model.numcomponents, nlevels);
use_pca = 1;
s = 0;  % holds the amount of temp storage needed by cascade()
for i = 1:length(pyra.scales)
    s = s + size(pyra.feat{i},1)*size(pyra.feat{i},2);
    if i > model.interval
        if use_pca
            scores = fconv(projpyra.feat{i}, rootfilters, 1, numrootfilters);
        else
            scores = fconv(pyra.feat{i}, rootfilters_0, 1, numrootfilters);
        end
        for c = 1:model.numcomponents
            u = model.components{c}.rootindex;
            v = model.components{c}.offsetindex;
            rootscores{c,i} = scores{u} + model.offsets{v}.w;
            numrootlocs = numrootlocs + numel(scores{u});
        end
    end
end

s = s*length(model.partfilters);
model.thresh = thresh;
% run remaining cascade stages and collect object hypotheses
if use_pca
    coords = cascade(model, pyra.feat, projpyra.feat, rootscores, ...
        numrootlocs, pyra.scales, pyra.padx, pyra.pady, s);
else
    coords = cascade(model, pyra.feat, pyra.feat, rootscores, ...
        numrootlocs, pyra.scales, pyra.padx, pyra.pady, s);
end
boxes = coords';
dets = boxes(:,[1:4 end-1 end]);
%toc