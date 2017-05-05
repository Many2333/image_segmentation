function [sz0,sz2] = get_sz(sh0,sv0);
sh2  = 2^ceil(log2(sh0));
sv2  = 2^ceil(log2(sv0));
sz0  = double([sh0,sv0]);
sz2  = double([sh2,sv2]);