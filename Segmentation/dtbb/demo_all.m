maxNumCompThreads(1);

%% Setup model structures
im            = imread('000084.jpg');
thr           = -.5;    %% detection threshold
obj_id        = 2;      %% second category (bicycles)
nw            = 256;    %% vocabulary size for lookup-based approximation
[object_model,models_cell,csc_model_true] = model_for_ubb(obj_id,nw,thr);

p_e           = .1;     %% probability that Chebyshev bound is not valid
fct_err       = sqrt(1/p_e);

%%=========================================================================
%% Bounding-based detection                                               %
%%=========================================================================

%%-------------------------------------------------------------------------
%% exact DTBB, first best
%%-------------------------------------------------------------------------
[dets_1b_ex,boxes_1b_ex,is_1b_ex,times_1b_ex,tr_1b_ex,domain_tree] = dtbb(im,object_model,thr, 1);

%%-------------------------------------------------------------------------
%% exact DTBB, all above thr
%%-------------------------------------------------------------------------
[dets_th_ex,boxes_th_ex,is_th_ex,times_th_ex,tr_th_ex,~]           = dtbb(im,object_model,thr,-1);

%%-------------------------------------------------------------------------
%% lookup-bound DTBB, first best
%%-------------------------------------------------------------------------
[dets_1b_lu,boxes_1b_lu,is_1b_lu,times_1b_lu,tr_1b_lu,~]           = dtbb(im,object_model,thr, 1,fct_err);

%%-------------------------------------------------------------------------
%% lookup-bound DTBB, all above thr
%%-------------------------------------------------------------------------
[dets_th_lu,boxes_th_lu,is_th_lu,times_th_lu,tr_th_lu,~]           = dtbb(im,object_model,thr,-1,fct_err);

%%-------------------------------------------------------------------------
%% lookup-bound cascade, all above thr
%%-------------------------------------------------------------------------
[times_clup,dets_clup,boxes_clup]                                  = cascade_lup(im,object_model,thr,fct_err);


%%=========================================================================
%% ORIGINAL DPM/C-DPM code                                                %
%%=========================================================================

%%-------------------------------------------------------------------------
%% GDTs
%%-------------------------------------------------------------------------

models_cell{1}.dofast = ~ispc;     %% uses (single threaded) SSE convolution code, from voc-release 5
%% on a windows machine you'll need to set the flag above to 0 (could not compile)

[dets_gdt, boxes_gdt, info_0,times_gdt,model]                      = imgdetect(im, models_cell{1}, thr);
%%-------------------------------------------------------------------------
%% PCA-based cascade (single-threaded), all above thr
%%-------------------------------------------------------------------------
[times_cdpm,dets_cdpm,boxes_cdpm]                                  = cascade_dpm(im,csc_model_true{1},thr);


%%=========================================================================
%% display timings
%%=========================================================================

disp('------DTBB, exact scores------')
disp(sprintf('HOG Pyramid         : %.2f sec', times_1b_ex(1)));
disp(sprintf('Score   computation : %.2f sec', times_1b_ex(2)));
disp(sprintf('KD tree construction: %.2f sec', times_1b_ex(3)));
disp(sprintf('Detection, 1st-best : %.2f sec', times_1b_ex(4)));
disp(sprintf('Detection, All>%.1f : %.2f sec', thr,times_th_ex(4)));
fprintf('\n')

disp('----DTBB, lookup-bound scores-')
disp(sprintf('HOG Pyramid + ANN   : %.2f sec', times_1b_lu(1)));
disp(sprintf('Feature computation : %.2f sec', times_1b_lu(2)));
disp(sprintf('KD tree construction: %.2f sec', times_1b_lu(3)));
disp(sprintf('Detection, 1st-best : %.2f sec', times_1b_lu(4)));
disp(sprintf('Detection, All>%.1f : %.2f sec', thr,times_th_lu(4)));
fprintf('\n')

disp('------------GDT---------------')
disp(sprintf('Feature computation : %.2f sec', times_gdt(1)));
disp(sprintf('GDT cost            : %.2f sec', times_gdt(2)));
fprintf('\n')


disp('-------PCA-based Cascade------')
disp(sprintf('HOG Pyramid         : %.2f sec', times_cdpm(1)));
disp(sprintf('Cascade detection   : %.2f sec', times_cdpm(2)));
fprintf('\n')

disp('------Lookup-bound Cascade----')
disp(sprintf('HOG Pyramid + ANN   : %.2f sec', times_clup(1)));
disp(sprintf('Cascade detection   : %.2f sec', times_clup(2)));
fprintf('\n')


is_terminal = isempty(com.mathworks.mde.desk.MLDesktop.getInstance.getMainFrame);
if is_terminal, break; end

%%=========================================================================
%% verify that all methods get the same detection & score
%%=========================================================================
dets_meth  = {dets_1b_ex, dets_1b_lu, dets_gdt,  dets_clup, dets_cdpm };
boxes_meth = {boxes_1b_ex,boxes_1b_lu,boxes_gdt, boxes_clup,boxes_cdpm};
methods    = {'DTBB - exact', 'DTBB - lookup bounds','GDT', 'Cascade, PCA-based','Cascade, lookup bounds'};
for k=1:5
    figure(k); clf;
    imshow(im); hold on;
    showboxes_on_im(boxes_meth{k}(1,:),'b')
    showboxes_on_im(dets_meth{k}(1,:), 'r')
    title([methods{k},sprintf('; score = %.4f',dets_meth{k}(1,end))],'fontsize',20)
end


%%=========================================================================
%% show upper bounds on intervals 
%%=========================================================================

%% mixture and scale indexes for the 1st-best detection 
mxt_best    = is_1b_ex{1}(1,end-1);
scl_best    = is_1b_ex{1}(1,end-2);

%% get DPM's dense score 
score_gdt       = model.rules{model.start}(mxt_best).score{scl_best+10};
sz              = size(score_gdt); 
offsets         = model.maxsize+1;

%% bounds computed up to first-best BB detection,   with exact part scores  
bounds_1b_ex       = computed_bounds(mxt_best,scl_best,domain_tree,tr_1b_ex,offsets,sz); 

%% bounds computed up to score = -1,                with exact part scores 
bounds_m1_ex        = computed_bounds(mxt_best,scl_best,domain_tree,tr_th_ex,offsets,sz); 

%% bounds computed up to first-best BB detection,   with lookup-based bounds
bounds_1b_lu       = computed_bounds(mxt_best,scl_best,domain_tree,tr_1b_lu,offsets,sz); 

%% bounds computed up to score = -1,                with lookup-based bounds
bounds_m1_lu        = computed_bounds(mxt_best,scl_best,domain_tree,tr_th_lu,offsets,sz);

fs    = 15;
range = [-2.5,1];
figure,imagesc(score_gdt, range);     axis off; title('GDT score','fontsize',fs)
figure,imagesc(bounds_1b_ex, range);  axis off; title('BB  upper bounds, 1st-best, exact part scores','fontsize',fs)
figure,imagesc(bounds_m1_ex, range);  axis off; title(sprintf('BB  upper bounds, Score>%.2f, exact part scores',thr),'fontsize',fs)
figure,imagesc(bounds_1b_lu, range);  axis off; title('BB  upper bounds, 1st-best, lookup-bounds for part scores','fontsize',fs)
figure,imagesc(bounds_m1_lu, range);  axis off; title(sprintf('BB  upper bounds, Score>%.2f, lookup-bounds for part scores',thr),'fontsize',fs)

