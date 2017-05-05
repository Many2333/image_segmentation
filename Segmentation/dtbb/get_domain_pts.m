
function [grh,grv,sz0,sz2,idx] = get_domain_pts(h_s,h_e,v_s,v_e,feat);
h_range     = h_e - h_s + 1;
v_range     = v_e - v_s + 1;

[grh,grv]   = my_meshgrid(h_s:h_e,v_s:v_e);
[sz0,sz2]   = get_sz(h_range,v_range);

% (physical memory coordinates) 
%  1D indexes of offsets for hog cells in template
idx         = uint16((grh-1)*size(feat,1) + grv);
% 2) horizontal/vertical offsets for template blocks
