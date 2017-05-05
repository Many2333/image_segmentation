function [normed,nrm] = normalize(input,nrm);
if nrm>0,
    nrm     = max((sum(input.^nrm,1)).^(1/nrm),eps);
    normed  = input./repmat(nrm,[size(input,1),1]);
else
    nrm     = ones(1,size(input,2));
    normed  = input;
end