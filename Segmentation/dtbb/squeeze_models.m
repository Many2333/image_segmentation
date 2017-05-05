function [subms,filters_out,model_out] = squeeze_models_full(modelc,vocab)
%[subms,filters,model_out] = squeeze_models(modelc)
% squeezes together multiple object category models and distills
% information used in BB optimization.
%
% Input: modelc: cell of object category models)
% Output:
% subms: an array of structures corresponding to the different model components
%            e.g. for a single-element modelc - one category, this will
%            have 6 elements for the object-specific mixtures
%            for a 20-element modelc this will have 120 elements
%
% filters_out:  contains all the root and all the part filters (.allr, .allp fields)
% model_out:    contains some useful info for constructing a common feature pyramid
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2


cnt = 0;
mxs = 0;
for md = 1:length(modelc)
    model = modelc{md};
    
    name    = model.class;
    symbols = model.symbols;
    filters = model.filters;
    rules   = model.rules;
    model_out.interval = model.interval;
    rules_generating_object = rules{model.start};
    nmixtures = length(rules_generating_object);
	
	if (nargin==1), vocab = [];  end
	mxts = [1:nmixtures];
    for mixture_ind = mxts,
        cnt              = cnt+1;
        mixture          = rules_generating_object(mixture_ind);
        symbols_combined = mixture.rhs; sms = symbols_combined;
        anchors          = mixture.anchor;
        sw               = mixture.detwindow;
        root_mixt        = symbols_combined(1);
        [root_w,f,s]     = get_Terminal(symbols,filters,rules,root_mixt);
        fs(1)  = f;
        
        pidx    = symbols_combined(1);
        def     = rules{pidx}.def.w;
        sch     = -1/sqrt(2*def(1));      scv = -1/sqrt(2*def(3));
        sigmas  = [sch,scv];
        bandwidth = single(pow_2(sigmas/(2)));
        
        relations(1).offsets = [0,0];
        relations(1).bandwidth  = bandwidth;
        n_parts = length(symbols_combined)-1;
        
        idx_box{1} = (mixture_ind-1)*4 + [1:4];
        
        for pi = 1 + [1:n_parts];
            pidx            = symbols_combined(pi);
            [w,f]           = get_Terminal(symbols,filters,rules,pidx);
            part_w{pi-1}    = w;
            filt            = filters(f);
            fs(pi)          = f;
            
            def  = rules{pidx}.def.w;
            part_d(pi-1,:)     = def;
            vecoff(pi-1,[1,2]) = anchors{pi}([1,2]);
            
            %% dt- potential:  def(1) h^2 + def(2) h + def(3) * v^2 + def(4) *v
            %% gaussian potential:   (h - mh)^2/(2*sch^2) + (v-mv)^2/(2*scv^2)
            
            %% max of dt-potential -> set h = -mh, v = -mv and get:
            max_dt(pi) =  (- def(2)^2/(4*def(1))) + ( - def(4)^2/(4*def(3)));
            %% as the gaussian potential used in BB has max 0, we need
            %% to add this to the the DC of the svm to make different mixtures
            %% commensurate
            
            sch = -1/sqrt(2*def(1));     %% sigma (h/v) corresponding to dt -based gaussian potential (defined in part coordinates)
            scv = -1/sqrt(2*def(3));
            
            mh =  -(def(2)/def(1))/2;     %% mean (h/v) corresponding to dt -based gaussian potential (defined in part coordinates)
            mv =  -(def(4)/def(3))/2;
            
            mh  = mh - anchors{pi}(1);   %% append this to the offset due to `anchor' field (again in part coordinates)
            mv  = mv - anchors{pi}(2);
            
            offset_0 = [mh,mv];
            sigmas   = [sch,scv];
            
            %% work in root coordinates -> cut everything down by 2
            offset    = single(offset_0)/2;
            bandwidth = single(pow_2(sigmas/(2)));
            
            relations(pi).offset     = offset;
            relations(pi).bandwidth  = bandwidth;
            idx_box{pi} = nmixtures*4 + (pi-2)*4 + [1:4];
        end
        
        [sv,sh,d]  = size(root_w);
        %% 1) max root resolution: 1/2 of max part resolution
        %% 2) HoG bin size used at finest pyramid level: 8/2
        %% -> min root granularity: (8/2)*2 = 8 pixels
        
        svm_b  = mixture.offset.w - sum(max_dt);%% used for DTBB
        svm_b0 = mixture.offset.w;				%% used for cascade
        nmax = nmixtures*9*4;
        fields_wt = {'md','mixture_ind','root_w','part_w','part_d','svm_b','svm_b0','sw','vecoff','fs','anchors','sms','n_parts','relations','idx_box','nmax','name'};
        compress_structure;
        subms(cnt) = structure;
    end
    mxs = max(mxs,modelc{md}.maxsize);
end

model_out.maxsize  = mxs;
model_out.interval = model.interval;
model_out.sbin     = 8;
nsubms  = length(subms);


cn = 0;
for mixt = 1:nsubms
    subm   = subms(mixt);
    nparts = length(subm.part_w);
    [weights_root{mixt},szs(:,mixt),en_root{mixt},...
        in_root{mixt}] = post_process(subm.root_w,vocab);
    w_root{mixt} = single(permute(subm.root_w,[3,1,2]));
    for pt = 1:nparts
        [weights_part{mixt}(pt,:),~,en_part{mixt}(pt,:),...
            in_part{mixt}(:,:,pt)] = post_process(subm.part_w{pt},vocab);
        cn = cn +1;
        w_part{cn} = single(permute(subm.part_w{pt},[3,1,2]));
    end
end

%%
%% weights_part
%% cell{n_mixtures}
%% each cell:
%% (nparts)  X (nv_hog x nh_hog x 32) elements ( = 6 x 6 x 32 = 1152)

%% in_part
%% cell{n_mixtures}
%% each cell:
%% (nwords) X  (nv_hog x nh_hog) X nparts

maxsize = max(szs,[],2);
pady    = maxsize(1);
padx    = maxsize(2);
%% feature dimensionality
nf    = size(subm.root_w,3);
%% max index of root filter
mxs   = padx*pady*nf;
nrmr = zeros(nsubms,prod(maxsize));

%% loop over root filters
for k=1:nsubms,
    %% root size
    sz_fil    = szs(:,k);
    %% find indexes for HOG-blocks in root template
    [grh,grv] = my_meshgrid(0:sz_fil(2)-1,0:sz_fil(1)-1);
    idxs      = grh*pady + grv+1;
    
    nrmr(k,idxs) = en_root{k};
    %% common-root dimension will be (padx,pady,nf)
    %% where padx,pady are maxima over mixture dimensions
    %% for every mixture, we fill in with weight vector values
    %% wherever available, and leave at zero elsewhere
    idxs             = repmat(idxs',[1,nf]) + repmat([0:nf-1],[length(idxs),1])*padx*pady;
    all_root(mxs,k)  = 0;
    all_root(idxs,k) = weights_root{k};
    sep_root{k}      = single(weights_root{k}(:));
end

%% weights_part
%% cell{n_mixtures}
%% each cell:
%% (nparts)  X (nv_hog x nh_hog x 32) elements ( = 6 x 6 x 32 = 1152)

filters_out.allp  = single(cell2mat(weights_part(:))');

%% 1152 x 48 = (6 x 6 x 32)

%%  all_root:
%% (nv_root x nh_root x 32) X (n_mixtures)
filters_out.allr  = single(all_root);

if ~isempty(vocab),
    nrmp = cell2mat(en_part(:));    
    szp = size(in_part{1});
    szl  = szp.*[1,1,length(in_part)];
    szl  = szl([3,1,2]);
    c = zeros(szl,'single');
    %% in_part
    %% cell{n_mixtures}
    %% each cell:
    %% (nwords) X  (nv_hog x nh_hog) X nparts
    for k=1:length(in_part),
        permuted   = permute(in_part{k},[3,1,2]);
        %% nparts  x (nwords) X  (nv_hog x nh_hog)
        c((k-1)*szp(3) + [1:szp(3)],:,:) = permuted;
        for i = 1:size(permuted,1),
            c_separate{(k-1)*szp(3) + i}  = permuted(i,:,:);
            %ep_separate{(k-1)*szp(3) + i} = 
        end
    end
    spy = 6; spx = 6;
    
    %% c:
    %% (nparts x nmixt) X nwords X (nv_p x nh_p)
    
    nmixt = length(in_root);
    d = zeros(nmixt,szp(1),padx*pady);
    nw = size(vocab,2);
    mx_dims  = max(szs,[],2);
    size_max = mx_dims(1);
    for k=1:nmixt,
        %% root size
        sz_fil    = szs(:,k);
        st = size_max - (sz_fil(1));
        %% find indexes for HOG-blocks in root template
        [grh,grv] = my_meshgrid([0:sz_fil(2)-1],st+[0:sz_fil(1)-1]);
        idxs      = grh*pady + grv+1;
        %% 1 (mixture)  x (nwords) X  (nv_hog x nh_hog)
        d(k,:,idxs)        = permute(in_root{k},[3,1,2]);
        d_separate{k}(1,:,:)    = in_root{k};
    end
   
    %% d:
    %% (1 x nmixt) X nwords X (nv_r x nh_r)
    
    sd = size(d);
    sc = size(c);
    %example sc: [960, 256, 36]
    if 1==1,
        permuted =  permute(c,[1,3,2]);
        % (nparts x nmixt) X (nv_p x nh_p) X nwords
        % [960,    36, 256] 
        
        reshaped = reshape(permuted,[sc(1)*spy,   spx,   nw]);
        % (nparts x nmixt x nv_p) X nh_p X nwords
        % [960 x 6, 6, 256]
        
        c = permute(reshaped,[1,3,2]);
        % (nparts x nmixt x nv_p) X nwords X nh_p
        % [960 x 6, 256, 6]

        %% same thing
        d = permute(reshape(permute(d,[1,3,2]),[sd(1)*pady,padx,nw]),[1,3,2]);
        
    end
    for k=1:length(d_separate),
        d_sep{k} = permute(reshape(permute(d_separate{k},[1,3,2]),[szs(1,k),szs(2,k),nw]),[1,3,2]);
    end
    for k =1:length(c_separate),
        c_sep{k} = permute(reshape(permute(c_separate{k},[1,3,2]),[6 ,  6,   nw]),[1,3,2]);
    end
    filters_out.scores_root          = single(d);
    filters_out.scores_part          = single(c);
    
    for k=1:length(en_root),
        er_sep{k} = single(reshape(en_root{k},szs(:,k)'));
    end
    cn  = 0;
    for p=1:length(en_part),
        for k = 1:size(en_part{p},1)
            cn = cn  + 1;
            ep_sep{cn} = single(reshape(en_part{p}(k,:),[6,6]));
        end
    end
    filters_out.scores_root_separate =  d_sep;
    filters_out.energy_root_separate = er_sep;
    filters_out.scores_part_separate =  c_sep;
    filters_out.energy_part_separate = ep_sep;
    filters_out.sizes_root_separate  = szs;
    filters_out.nrmr                 = single(nrmr);
    filters_out.nrmp                 = single(nrmp);
end
filters_out.maxsize = maxsize;
filters_out.w_part  = w_part;
filters_out.w_root  = w_root;

%% precomputed deformation costs (for cascade)
[grh,grv] = meshgrid([-5:5],[-5:5]);
grh = -grh;
grv = -grv;
for c = 1:length(subms)
    for part = 1:size(subms(c).part_d,1),
        df          = subms(c).part_d(part,:);
        sch         = -single(df(1)*grh.^2 + df(2)*grh);
        scv         = -single(df(3)*grv.^2 + df(4)*grv);
        subms(c).costs{part}       = sch(:) + scv(:);
    end
end


function [weight,sz,nr2,inpr] = post_process(in,vocab)
%% The returned weight is not permuted.
%% But we permute the norm of the weight elements
%% and their responses to the codebook entries 
%% (it makes accessing them in batch mode possible with 
%% fewer cache misses)
[sv,sh,d]   = size(in);
weight      = reshape(in,[1,sv*sh*d])';
sz          = [sv,sh];

in_lett = reshape(in(end:-1:1,:),[sv*sh,d]);
nr2     = squeeze(sum(in_lett.^2,2)/size(in_lett,2));
inpr 	= [];
if ~isempty(vocab),
    inpr    = (in_lett*vocab)';
end

function [w,f,symb] = get_Terminal(symbols,filters,rules,Terminal)
symb    = rules{Terminal}.rhs;
f       = symbols(symb).filter;
w       = filters(f).w;

%% debugging leftovers: allows me to use block cells one-by-one 
if 1==2
    %if size(w,2)>8,
    %    keep_h = [1:size(w,2)];
    %else
    keep_h = 1;
    keep_v = 1;
    %end
    %keep_v = 1:size(w,1);
    w_keep =  w(keep_v,keep_h,:);
    w = w*0;
    w(keep_v,keep_h,:) = w_keep;
end


function x = pow_2(x)
x = x.*x;

