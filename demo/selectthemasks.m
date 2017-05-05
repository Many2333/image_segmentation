% fuction select the masks
% by lingzhengdai 



function    [sele1,selemasks] = selectthemasks ( A, masks , areabounding,im) 
  sele1 = 0;
        for j = 1 : size(masks,3)
            if ( sum(sum(masks(:,:,j).*A))/sum( sum(masks(:,:,j)) ) )>0.7  && ...
            ( sum(sum(masks(:,:,j).*A)) / areabounding )<=0.8 && ( sum (sum(masks(:,:,j).*A)) / areabounding )>=0.4
        
               sum (sum(masks(:,:,j)));
                %[Imgs] =
       % figure;  
        subplot_auto_transparent(masks(:,:,j).*A, im);
                 sele1 = sele1+1;
                 selemasks(:,:,sele1) = masks(:,:,j).*A;
               %  selemasks(:,:,sele1) = masks(:,:,j).*A;
                 
            end
        end

          if sele1==0
            for j = 1 : size(masks,3)
              if  ( sum(sum(masks(:,:,j).*A))/sum( sum(masks(:,:,j)) ) )>0.7  && ... 
                  ( sum(sum(masks(:,:,j).*A)) / areabounding )<=0.9 && ( sum (sum(masks(:,:,j).*A)) / areabounding )>0.2
              subplot_auto_transparent(masks(:,:,j).*A, im);
              sele1 = sele1+1;
              selemasks(:,:,sele1) = masks(:,:,j).*A;
              end
            end
          end
         if sele1==0
            for j = 1 :  size(masks,3)
             % if  ( sum(sum(masks(:,:,j).*A))/sum( sum(masks(:,:,j)) ) )>0.7  && ... 
              %    ( sum(sum(masks(:,:,j).*A)) / areabounding )<0.8 && ( sum (sum(masks(:,:,j).*A)) / areabounding )>0
              if ( sum (sum(masks(:,:,j).*A)) / areabounding )>0
                  subplot_auto_transparent(masks(:,:,j).*A, im);
              sele1 = sele1+1;
              selemasks(:,:,sele1) = masks(:,:,j).*A;
              end
             % end
            end
         end
end
          