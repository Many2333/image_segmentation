function [object_model,models_cell,csc_model_true] = model_for_ubb(obj_id,nw,thrs_in,nrm_hog)
if nargin<4
    nrm_hog = 0;
end
if nargin==2,
    thrs_in = [];
    thrsc = []; thresh = []; ords = [];
end

vocab               = get_vocabulary(nw,nrm_hog);
single_object       = obj_id;
[models_cell]       = load_models(single_object);
models_cell_0       = models_cell;
[submodels, object_model,model0]  ...
                    = squeeze_models(models_cell,vocab);

for k=1:length(models_cell)
    csc_model_true{k} = cascade_model(models_cell_0{k}, '2009', 5,0);
    for thr_ind = 1:length(thrs_in),
        cn = 0;
        thr = thrs_in(thr_ind);
        csc_model = cascade_model(models_cell_0{k}, models_cell_0{k}.year, 0, thr);
        for sm = 1:length(csc_model.cascade.t),
            cn  = cn  + 1;
            thrs(cn,:) = csc_model.cascade.t{sm}';
            ords(cn,:) = csc_model.cascade.order{sm}';
        end
        thrsc{thr_ind} = thrs;
        dum = cascade_model(models_cell_0{k}, '2009', 5,thr);
        csc_model_true{k}.cascadec{thr_ind} = dum.cascade;
    end
    csc_model_true{k}.thrs = thrs_in;
end


object_model.blocksize_part = [6,6];
object_model.blocksize_root = object_model.maxsize';
object_model.model          = model0;
object_model.submodels      = submodels;
object_model.vocab          = vocab;
object_model.thrsc          = thrsc;
object_model.thresh         = thrs_in;
object_model.ords           = ords;
object_model.nrm_hog        = nrm_hog;


nsubm  = length(submodels);
cn = 0;
bwp = zeros(2,submodels(1).n_parts*nsubm,'single'); offp= bwp;
bwr = zeros(2,nsubm,'single');                      offr= bwr;
for mixt =  1:nsubm
    submodel     = submodels(mixt);
    relations    = submodel.relations;
    offr(:,mixt) = zeros(2,1,'single');
    bwr(:,mixt)  = 1./relations(1).bandwidth;
    for part = [1:submodel.n_parts],    %% loop over mixture parts
        cn = cn + 1;
        bwp(:,cn)   = 1./relations(part+1).bandwidth;
        offp(:,cn)  = relations(part+1).offset;
    end
    bs(mixt)  = single(submodel.svm_b);
    bs0(mixt) = single(submodel.svm_b0);
end

object_model.bs      = bs;
object_model.bs0     = bs0;
object_model.offsp  = offp;
object_model.offsr  = offr;
object_model.bwr    = bwr;
object_model.bwp    = bwp;

scores_part_separate = object_model.scores_part_separate;
scores_root_separate = object_model.scores_root_separate;

for pid = [1:length(scores_part_separate)]
    scp{pid}  = reshape(permute(flipdim(scores_part_separate{pid},1),[2,1,3]),[nw,36]);
end
for pid = [1:length(scores_root_separate)]
    scr{pid}  = reshape(permute(flipdim(scores_root_separate{pid},1),[2,1,3]),[nw,prod(object_model.submodels(pid).sw)]);
end

object_model.scp = scp;
object_model.scr = scr;


function [models_cell] = load_models(single_object);
% [models_cell] = load_models(single_object);
% returns a cell array containing the DPM structures
% if single_object >0  returns cell with single object
% otherwise returns cell array with all 20 object categories of DPM 08/10
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2

DPM_code_dir  = fullfile(fileparts(mfilename('fullpath')),'voc-release4');
dirstart  = fullfile(DPM_code_dir,'VOC2009');
models    = dir(fullfile(dirstart,'*_final.mat'));

if single_object ==0,
    objects = [1:20];
else
    objects = single_object;
end
cn = 0;
models_cell = [];
%% construct cell array of models
for k= objects
    load(fullfile(dirstart,models(k).name));
    %% use common lowest threshold for all models
    model.thresh    = min(-1.1, model.thresh);
    cn = cn + 1;
    models_cell{cn}      = model;
end


function vocab = get_vocabulary(nwords,do_norm)
fnm_0 = 'vocab_1e5_';
rt = fileparts(mfilename('fullpath'));

if (nargin<=2)|(do_norm ==0)
    do_norm = 0;
    norm_str = '';
else
    norm_str = sprintf('norm_%i_',do_norm);
end

fnm_save = fullfile(rt,[fnm_0,norm_str,num2str(nwords)]);
try
    load(fnm_save,'vocab'); vocab; return;
end

%%----------------------------------------------------------------------
%% Codebook construction
%%----------------------------------------------------------------------
conf.dataDir = fullfile(matlabsavedir,'bb'); my_cd(conf.dataDir);
conf.imDir   = fullfile(matlabsavedir,'VOC','VOC2009','JPEGImages');

conf.numTrain    = 50;
conf.numWords    = nwords;
conf.quantizer   = 'kdtree' ;
conf.hogOpts     = {'Step', 5} ;
conf.clobber     = false ;

conf.tinyProblem = false;
conf.prefix = 'try_000' ;
conf.randSeed = 1 ;

conf.vocabPath   = fullfile(conf.dataDir, [conf.prefix '-vocab.mat']) ;
conf.histPath    = fullfile(conf.dataDir, [conf.prefix '-hists.mat']) ;
conf.modelPath   = fullfile(conf.dataDir, [conf.prefix '-model.mat']) ;
conf.resultPath  = fullfile(conf.dataDir, [conf.prefix '-result']) ;

randn('state',      conf.randSeed) ;
rand('state',       conf.randSeed) ;
vl_twister('state', conf.randSeed) ;

% --------------------------------------------------------------------
%                                                           Setup data
% --------------------------------------------------------------------
ims = vl_colsubset(dir(fullfile(conf.imDir,'*.jpg'))',conf.numTrain);
ims = cellfun(@(x)fullfile(conf.imDir,x),{ims.name},'UniformOutput',false);

conf.ncells = 1e4; % number of HOG cells used for dictionary construction
conf.cpi    = ceil(conf.ncells/conf.numTrain); % number of cells per image
hog.sbin = 8; hog.interval = 10; padx = 0; pady = 0;

sampled = [];
for im_id = 1:length(ims),
    fprintf(2,'.');
    im   = (double(imread(ims{im_id})))/255;
    pyra = featpyramid(im, hog,padx,pady);
    %pyra = projectpyra(im,
    out = [];
    for k=1:length(pyra.feat),
        out = [out,reshape_pyra(pyra.feat{k})'];
    end
    sampled = [sampled,vl_colsubset(out,conf.cpi)];
end

if do_norm>0,
    [sampled,nr] = normalize(sampled,do_norm);
end

for nw = [64,128,256,512,1024,2048]
    fnm_save =  fullfile(rt,[fnm_0,pca_str,norm_str,num2str(nw)]);
    try
        load(fnm_save);
    catch
        [vocab0,i]  = vl_kmeans(single(sampled),  nw, 'verbose', 'algorithm', 'elkan','NumRepetitions',10);
        ds = sqrt(sum(pow_2(sampled -  vocab0(:,i)),1));
        [hs] = histc(i,[1:nw+1]-.5); hs = hs(1:end-1);
        [sr,id] = sort(hs,'descend');
        vocab = vocab0(:,id);
        save(fnm_save,'vocab','ds','i','id');
    end
end
return