% the core programe of SPAr Reconstruction
% ||m-My||_1

function recErrorm  = calculateSPArmasks1 (dictionary1,allfeats1,paramSR,dictionaryM)

allfeats1 = normVector(allfeats1);                         % X
dictionary1 = normVector(dictionary1);                     % B                              
dictionaryM = dictionaryM;                    % M
global  mask 
mask = zeros(size(dictionaryM,1),size(dictionaryM,2));     % m
lambda2 = 0.01;
lambda3 = 0.01;
paramSR.lambda;
ro = 1;
eye(size(dictionaryM,1));                                  % I
zeros(size(dictionary1,1),size(size(dictionaryM,1)));      % 0




%%%%%%%%%%%%%%%%%%%  solve for y and mask--m
 for mi = 1:10
     
% for Mi = 1:size(dictionaryM,1)
%     nM(Mi,:) = sum(dictionaryM);                           % (1/n * M) * n 
% end
% mask = nM;
allfeats = [allfeats1 ; sqrt(ro)*lambda2*mask ];           % v~
II1 = [zeros(size(dictionary1,1),size(dictionaryM,1)); sqrt(ro)*paramSR.lambda*eye(size(dictionaryM,1))];
size(II1);
II2 = [dictionary1;sqrt(ro)*lambda2*dictionaryM];
size(II2);
dictionary = [ [zeros(size(dictionary1,1),size(dictionaryM,1)); sqrt(ro)*paramSR.lambda*eye(size(dictionaryM,1))],[dictionary1;sqrt(ro)*lambda2*dictionaryM] ];

paramSR.L = length(allfeats(:,1)); 


betau = mexLasso(allfeats, dictionary, paramSR);           % u 
%mask = dictionaryM * beta; 
beta = betau((size(dictionaryM,1)+1):size(betau,1),:);     % y  
for Mi = 1:size(dictionaryM,1)
    nM(Mi,:) =1- sum(dictionaryM);                           % (1/n * M) * n 
end
W = nM/size(dictionaryM,1)-dictionaryM*beta;

kesi = lambda2/(2*lambda3);
XM = max(W-kesi,0) + min(W+kesi,0);                        % XM for m 
mask = XM + dictionaryM*beta;                              % figure-ground confidence map
end
%%%%%%%%%%%%%%%%%%%%%%


mask2 = size(dictionaryM,1) - mask;
betau = full(betau);
beta = betau((size(dictionaryM,1)+1):size(betau,1),:);      % y



%mask = (sum(dictionaryM(:))-dictionaryM)*beta;
% beta = (beta -   min(beta(:)))/(max(beta(:)) - min(beta(:)));
% beta = max(beta(:))-beta;


%figure; imshow (beta);

mask2;
%mask = dictionaryM*beta;
%figure; imshow (full(mask));
recErrorm = sum (mask2);
%figure; imshow(recErrorm );
%figure; imshow(dictionaryM);
%mask = betau((1:size(dictionaryM,1)),:); 
size(mask2);

%mask = nM;

recError = sum((allfeats1 - dictionary1*beta(1:size(dictionary1,2),:)).^2); 
%recError = sum((allfeats - dictionary*beta(1:size(dictionary,2),:)).^2); 

recError = (recError -   min(recError(:)))/(max(recError(:)) - min(recError(:)));

recErrorm = sum (mask2);
%recErrorm = sum(dictionaryM);
recErrorm = (recErrorm -   min(recErrorm(:)))/(max(recErrorm(:)) - min(recErrorm(:)));
recErrorm = max(recErrorm(:))-recErrorm;
recErrorm = 0*recErrorm + 1*recError;
 


