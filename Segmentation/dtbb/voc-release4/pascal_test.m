function boxes1 = pascal_test(cls, model, testset, year,nmax, suffix,sv_dir)

% boxes1 = pascal_test(cls, model, testset, year, suffix)
% Compute bounding boxes in a test set.
% boxes1 are detection windows and scores.

% Now we also save the locations of each filter for rescoring
% parts1 gives the locations for the detections in boxes1
% (these are saved in the cache file, but not returned by the function)

setVOCyear = year;
globals;
pascal_init;

ids = textread(sprintf(VOCopts.imgsetpath,testset), '%s');
ids = ids(1:min(end,nmax));

% run detector in each image
try
    load([sv_dir cls '_boxes_' testset '_' suffix]);
catch
    % parfor gets confused if we use VOCopts
    opts = VOCopts;
    parfor i = 1:length(ids);
        fprintf('%s: testing: %s %s, %d/%d\n', cls, testset, year, ...
            i, length(ids));
        if strcmp('inriaperson', cls)
            % INRIA uses a mixutre of PNGs and JPGs, so we need to use the annotation
            % to locate the image.  The annotation is not generally available for PASCAL
            % test data (e.g., 2009 test), so this method can fail for PASCAL.
            rec = PASreadrecord(sprintf(opts.annopath, ids{i}));
            im = imread([opts.datadir rec.imgname]);
        else
            im = imread(sprintf(opts.imgpath, ids{i}));
        end
        try
            switch model.meth,
                case 'csc_lu',
                    [~,dets,boxes] = imgdetect_csc_lookup(im,    model,model.thr,model.f);
                case 'bb',
                    [dets,boxes]   = imgdetect_bb(im,{model},model.thr,-1);
                case 'csc_rg',
                    [~,dets,boxes] = imgdetect_csc_dpm(im,model,model.thr);
                case 'bb_lu',
                    [~,dets,boxes] = imgdetect_bb_lookup(im,model,model.thr,-1,model.f);
                case 'gdt',
                    [dets,boxes]   =  imgdetect(   im, model,-1);
            end
        catch
            disp(lasterr);
            dets = [];
            boxes = [];
        end
        
        if ~isempty(boxes)
            boxes = reduceboxes(model, boxes);
            [dets boxes] = clipboxes(im, dets, boxes);
            I = nms(dets, 0.5);
            boxes1{i} = dets(I,[1:4 end]);
            parts1{i} = boxes(I,:);
        else
            boxes1{i} = [];
            parts1{i} = [];
        end
        %showboxes(im, boxes1{i});
    end
    save([sv_dir cls '_boxes_' testset '_' suffix], ...
        'boxes1', 'parts1');
end
