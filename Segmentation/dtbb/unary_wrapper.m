function [part_trees,root_trees,domain_kdtree,bs,times] = unary_wrapper(model,levels_wt,pyra,doa,interval,times,fct_err)
bwr             = model.bwr;
bwp             = model.bwp;
bs              = model.bs;
allr            = model.allr;
allp            = model.allp;
blocksize_root  = model.blocksize_root;
blocksize_part  = model.blocksize_part;
offsr           = model.offsr;
offsp           = model.offsp;

if doa,
    nrmr        = model.nrmr;
    nrmp        = model.nrmp;
    scores_root = model.scores_root;
    scores_part = model.scores_part;
end

if ~exist('times','var'), times = zeros(1,3); end

unary = tic;
for l = 1:length(levels_wt)
    level     = levels_wt(l);
    if doa,
        [root_trees{l},troot,grh,grv]  = get_unary(pyra,level,         allr,blocksize_root,bwr,offsr,bs,1,scores_root,nrmr,single(fct_err));
        [part_trees{l},tpart]          = get_unary(pyra,level-interval,allp,blocksize_part,bwp,offsp,[],1,scores_part,nrmp,single(fct_err));
    else
        [root_trees{l},troot,grh,grv]  = get_unary(pyra,level,         allr,blocksize_root,bwr,offsr,bs,0);
        [part_trees{l},tpart]          = get_unary(pyra,level-interval,allp,blocksize_part,bwp,offsp,[],0);
    end
    if l==1
        tic;
        domain_kdtree = get_domain_kdtree(grh,grv);
        times(3) = times(3) + toc;
    end
    times([2,3]) = times([2,3]) + troot  + tpart;
end

if 0
l = 1;
level     = levels_wt(l);
doa = 1;
[root_trees_lu{l},troot,grh,grv]  = get_unary(pyra,level,         allr,blocksize_root,bwr,offsr,bs,1,scores_root,nrmr,single(fct_err));
[part_trees_lu{l},tpart]          = get_unary(pyra,level-interval,allp,blocksize_part,bwp,offsp,[],1,scores_part,nrmp,single(fct_err));
doa = 0;
[root_trees_ap{l},troot,grh,grv]  = get_unary(pyra,level,         allr,blocksize_root,bwr,offsr,bs,0);
[part_trees_ap{l},tpart]          = get_unary(pyra,level-interval,allp,blocksize_part,bwp,offsp,[],0);
end