function B = regionMasks(supNum,sulabel, A)
A;            %  background is 0
C = 1-A;      %  background is 1  
%C = C-1;

B=zeros(1,supNum);   
for r = 1:supNum
	l1 = length(find(sulabel==r));
	l2 = 0;
    [indx indy] = find(sulabel==r);
    length(indx);
    for jj = 1: length(indx)
        if C(indx(jj),indy(jj))==1;
           % C(indx(j),indy(j))=0;
           l2 = l2+1;
%         else
%            C(indx(j),indy(j))=1;    
        end
        
    end
    
 
    if l2/l1 > 0.7
        
        B(1,r) = 1;
    else
        B(1,r) = 0;
    end
	%regions{r}.pixelInd = indxy;
    %regions{r}.pixelIndxy = [indx indy];
end
%figure ; imshow(C);

D=C-1;
for r = 1:supNum
    if B(1,r) == 0
        
   [indx indy] = find(sulabel==r);
   length(indx);
    for j = 1: length(indx)
         D(indx(j),indy(j))=1;
           %l2 = l2+1;
        
    end  
    end
end

%figure; imshow(D);

    
    
    
    
    
    

    