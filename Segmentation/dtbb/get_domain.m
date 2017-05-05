function [h_s,h_e,v_s,v_e,grfh,grfv,ng,offset] = get_domain(pyra,feat,sz_filter) 

padx = pyra.padx;
pady = pyra.pady;

sfx  = sz_filter(2);
sfy  = sz_filter(1);

[szv,szh,nd] = size(feat);
%sz_im(1)     = szv - 2*pady;   %% vertical
%sz_im(2)     = szh - 2*padx;   %% horizontal

%% IMAGE pixel (1,1) is at PYRAMID pixel (padx+1,pady+1)
%% to allow for partially occluded parts, DPMs start evaluating score
%% at image pixel (-smy, -smx),
%% so we need to start sampling at pady-smy , padx - smx

if 0, %((sfx==6)&&(sfy==6))
   %h_s = padx; 
  %  h_s = padx - sfx      + 3;
  %  h_e = padx + sz_im(2) + 1;
  %  v_s = pady - sfy      + 3;
  %  v_e = pady + sz_im(1) + 1;
  %  h_s = padx  + 1;
  %  h_e = szh  -sfx + 1; % -1 - sfx;
  %  v_s = pady  + 1;
  %  v_e = szv  -sfy + 1; % -1 - sfy;
else
    h_s = 1; %padx - sfx      + 1;
    h_e = szh -sfx + 1; % -1 - sfx;
    v_s = 1; %pady - sfy      + 1;
    v_e = szv -sfy + 1; % -1 - sfy;
end

if 0, % ~((sfx==6)&&(sfy==6)),
    h_s = padx - 2;
    v_s = pady - 2;
end

offset     = [v_s,h_s]  - 1;        %% used only for demo_0 script to bring unary scores into alignment
ng         = -[padx+1,pady+1];      %% adding this to PYRAMID coordinates turns into c-style IMAGE coordinates

% 2) horizontal/vertical offsets for template blocks
[grfh,grfv] = my_meshgrid(0:sfx-1,0:sfy-1);