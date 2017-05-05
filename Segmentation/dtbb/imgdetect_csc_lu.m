function [t,dets,boxes] = imgdetect_csc_lu(object_model,thrs,fct_err,pyra)

structure       = object_model; expand_structure;
interval        = model.interval;
levels_wt       = (interval+1):length(pyra.feat);

model_wanted     = 1;
component_wanted = 1:6;
level_wanted     = levels_wt;

tic
sz_root_max             = max(object_model.sizes_root_separate,[],2);
xs_all = [];
ys_all = [];
vs_all = [];
id_all = [];
l_all  = [];
sols = [];
padx = pyra.padx;
pady = pyra.pady;

for level  = level_wanted,
    i_root  = pyra.i{level};
    i_part  = pyra.i{level - 10};
    
    d_root  = pyra.d{level};
    d_part  = pyra.d{level - 10};
    
    f_root  = pyra.featp{level};
    f_part  = pyra.featp{level- 10};
    
    sz_part = size(i_part);
    sz_root = size(i_root);
    
    rnds    = [ones(1,9),2*ones(1,9)];
    vals0   = zeros(9,prod(sz_part),'single');
    
    [hr_s,hr_e,vr_s,vr_e]   = get_domain(pyra,i_root,sz_root_max);
    [hp_s,hp_e,vp_s,vp_e]   = get_domain(pyra,i_part,[6,6]);
    
    [siv,sih] = size(i_root);
    %[hs,vs]  = meshgrid_fast([1:sih],[1:siv]);
    
    for c = 6*(model_wanted-1) + component_wanted
        sc_root_sep             = scores_root_separate{c};
        sz_root_sep             = sizes_root_separate(:,c)';
       
        visit_order         = ords(c,:)+1;
        thresholds          = thrs(c,2:2:end) - bs0(c);
        thresholds_def      = thrs(c,3:2:end) - bs0(c);

        anchors             = submodels(c).anchors;
        
        cnt         = 1;
        part        = visit_order(cnt)-1;
        rnd         = rnds(cnt);
        threshold   = thresholds(cnt);
        
        Nv                  = sz_root_sep(1);
        Nh                  = sz_root_sep(2);
        %%---------------------------------------------------------
        %% Root filter
        %%---------------------------------------------------------
        %% very first call: dense lookup everywhere
        if (rnd==2)
            %% determine area of operation (in `memory' coordinates)
            feat_root =  pyra.feat{level};
            [h_s,h_e,v_s,v_e,grfh,grfv,ng] = get_domain(pyra,feat_root,sz_root_max);
            [grh,grv,sz0,sz2,idx]          = get_domain_pts(h_s,h_e,v_s,v_e,feat_root);
            t       = object_model.allr(:,c);
            sv      = v_e - v_s + 1;
            sh      = h_e - h_s + 1;
            inpr    = reshape((getfeats(feat_root,idx,grfh,grfv)*t)',[sv,sh]);
        else
            en_root     = energy_root_separate{c};
            [inpr,dst]  = fast_lookup_call(i_root,sc_root_sep,Nv,Nh,hr_s,hr_e,vr_s,vr_e,d_root,en_root,fct_err);
        end
        idxs                = find(inpr > threshold);
        vals                = inpr(idxs);
        [v,h]               = ind2sub_fast(size(inpr),idxs);
        if isempty(v)
            break;
        end
        
        wt_now              = idxs;
        
        sv = vr_e - vr_s + 1; sh = hr_e - hr_s + 1;
        vals0(1,wt_now)     = vals;
        
        for cnt = 2:length(visit_order),
            part        = visit_order(cnt)-1;
            rnd         = rnds(cnt);
            threshold   = thresholds(cnt);
            
            if part>0
                %% Using the results from the current coarse level (l_c) we  want to
                %% guide the convolution at a finer level of the pyramid (l_f).
                %% Our reasoning about their positions is based on actual (image) coordinates,
                %% which we then translate into memory coordinates.
                
                %% An actual point [h,v] survives @l_c -> a point [2*h, 2*v] @l_f
                %% We have an anchor giving us the nomimal displacement @l_f
                %% -> point becomes [2*h + A_h, 2*v + A_j] @l_f
                %% -> In memory coordinates: [2*h + A_h + pad_h, 2*v +A_v + pad_v]
                %%
                %% Just to save some more computation, we now express this in terms of
                %% SampledMemory coordinates @l_c: point [h_m,v_m] survives -> transformations are
                %% [h,v] = [h_m -     Nh + 1,       v_m -  Nv + 1                   ]
                %% i.e. [1,1] -> corresponds to Actual Point [ -Nh +1, -Nv + 1]
                %%
                %% [2*h_m -     2*(Nh + 1),         2*v_m - 2*(Nh + 1)              ]
                %% [2*h_m -     2*(Nh + 1) + A_h,   2*v_m - 2*(Nh + 1) + A_v        ]
                %% [2*h_m -2*(Nh + 1)+ pad_h + A_h, 2*v_m + pad_v - 2*(Nv + 1) + A_v]
                
                pid         = part + (c-1)*8;
                costs       = submodels(c).costs{part};
                sclm        = 5;
                h2          = uint8(2*h + (- pyra.padx - 1 + anchors{part+1}(1)) -1 );
                v2          = uint8(2*v + (- pyra.pady - 1 + anchors{part+1}(2)) -1 );
                f_in        = f_part;
                w_in        = w_part{pid};
                en_part     = energy_part_separate{pid};
                
                left    = hp_s;
                right   = hp_e;
                up      = vp_s;
                down    = vp_e;
            else
                h2      = uint8(h-1)';
                v2      = uint8(v-1)';
                costs   = single(0);
                sclm    = 0;
                
                f_in    = f_root;
                w_in    = w_root{c};
                left    = hr_s;
                right   = hr_e;
                up      = vr_s;
                down    = vr_e;
            end
           
            thresh      = thresholds_def(cnt-1) - vals;
            
            do_lookup = (rnd==1);
            if do_lookup
                %% lookup-based approximation
                %% code valid exclusively for parts, not for root filter
                scores              = sparse_lookup(i_part,scp{pid}, sclm,v2,h2,left,right,up,down,costs,thresh,d_part,en_part,fct_err);
            else
                %% evaluate HOG convolution
                [scores,ix,iy]      = sparse_lookup(f_in,w_in,       sclm,v2,h2,left,right,up,down,costs,thresh);
                if cnt==10
                    xs      = zeros(length(ix),9);
                    ys      = zeros(length(ix),9);
                    xs(:,1) = ix - padx;
                    ys(:,1) = iy - pady;
                else
                    xs(:,part+1) = ix - padx;
                    ys(:,part+1) = iy - pady;
                end
            end
            
            if (cnt>9)
                scs_p      = vals0(cnt-9,  wt_now)';
                vals       = vals + (scores  - scs_p);
            else
                vals      = vals   + scores;
            end
            
            %% code for both root and parts
            wt         = find(vals>threshold);
            wt_now     = wt_now(wt);
            v          = v(wt);
            h          = h(wt);
            vals       = vals(wt);
            
            if isempty(v)
                break;
            end
            
            if (cnt<=9)
                vals0(cnt,wt_now) = scores(wt);
            else
                xs = xs(wt,:);
                ys = ys(wt,:);
            end
            if isempty(v), break; end
        end
        
        if ~isempty(v)
            onz = ones(size(vals));
            xs_all       = [xs_all;xs];
            ys_all       = [ys_all;ys];
            vs_all       = [vs_all;vals + bs0(c)];
            id_all       = [id_all;repmat([model_wanted,level ,c],[length(onz),1])];
        end
    end
end

t = toc;
if nargout>1,
    if ~isempty(id_all)
        scales = pyra.scales(interval+1:end);
        [dets,boxes] = sol2boxes_cascade(id_all',vs_all,submodels,scales,xs_all,ys_all);
       
    else
        dets = [];
        boxes = [];
    end
end