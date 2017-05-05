function domain_kdtree = get_domain_kdtree(grh,grv)
%
% Copyright (C) 2012 Iasonas Kokkinos
% available under the terms of the GNU GPLv2

grht = single(grh); grvt = single(grv);
mnh = min(grht);
mnv = min(grvt);
mxh = max(grht);
mxv = max(grvt);

[sz0,sz2]     = get_sz((mxh-mnh)+1,(mxv -mnv)+1);
domain_kdtree = mex_kdtree([grht;grvt],sz0,sz2,double(mnh)-1,double(mnv)-1);

function [sz0,sz2] = get_sz(sh0,sv0)
sh2  = 2^ceil(log2(sh0));
sv2  = 2^ceil(log2(sv0));
sz0  = double([sh0,sv0]);
sz2  = double([sh2,sv2]);