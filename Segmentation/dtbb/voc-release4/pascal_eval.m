function [ap,recall,prec] = pascal_eval(cls, boxes, testset, year, suffix,cls_name,sv_dir)

% ap = pascal_eval(cls, boxes, testset, suffix)
% Score bounding boxes using the PASCAL development kit.

setVOCyear = year;
globals;
pascal_init;
ids = textread(sprintf(VOCopts.imgsetpath, testset), '%s');

f_to_save  =sprintf(VOCopts.detrespath, 'comp3', cls);
f_to_save

% write out detections in PASCAL format and score
fid = fopen(f_to_save, 'w');
for i = 1:length(ids);
  bbox = boxes{i};
  for j = 1:size(bbox,1)
    fprintf(fid, '%s %f %d %d %d %d\n', ids{i}, bbox(j,end), bbox(j,1:4));
  end
end
fclose(fid);

VOCopts.testset = testset;
if str2num(VOCyear) == 2006
  [recall, prec, ap] = VOCpr(VOCopts, 'comp3', cls_name,cls, true);
elseif str2num(VOCyear) < 2008
  [recall, prec, ap] = VOCevaldet(VOCopts, 'comp3', cls_name,cls, true);
else
  recall = 0;
  prec = 0;
  ap = 0;
end
if 0
    if str2num(VOCyear) < 2008
        % force plot limits
        ylim([0 1]);
        xlim([0 1]);
        
        % save results
        save([sv_dir cls '_pr_' testset '_' suffix], 'recall', 'prec', 'ap');
        print(gcf, '-djpeg', '-r0', [sv_dir cls '_pr_' testset '_' suffix '.jpg']);
    end
end