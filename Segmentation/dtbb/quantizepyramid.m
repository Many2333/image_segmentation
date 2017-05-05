function [pyra] = quantizepyramid(pyra,vocab,do_approx,nrm_hog)
kdtree  = vl_kdtreebuild(vocab);
if nargin<=2, do_approx= 0; end
if nargin<=3, nrm_hog = 0; end

for k = 1:length(pyra.feat),
    pyra_in         = pyra.feat{k};
    sz              = size(pyra_in);
    pyra_sz         = reshape_pyra(pyra_in)';
    
    if nrm_hog
        [pyra_sz,n_hog]  = normalize(pyra_sz,nrm_hog);
        pyra.n{k}       = reshape((n_hog),sz([1,2]));
    else
        n_hog = 1;
        pyra.n{k}  =1;
    end
    [i,d]           = vl_kdtreequery(kdtree, vocab ,pyra_sz, 'MaxComparisons', 20);
    if size(vocab,2)>256
        pyra.i{k}       = uint16(reshape(i,sz([1,2]))-1);
    else
        pyra.i{k}       = uint8(reshape(i,sz([1,2]))-1);
    end
    pyra.d{k}       = reshape(d,sz([1,2]));
    err             = (pyra.feat{k} - reshape(vocab(:,i)',[sz]));
    pyra.d4{k}      = squeeze(sum(pow_2(pow_2(err)),3));
    
    pyra.featp{k}    = permute(single(pyra_in),[3,1,2]);
    if do_approx,
        if nrm_hog,
            pyra.feat_ap{k} = reshape((repmat(n_hog,[sz(3),1]).*vocab(:,i))',[sz]);
        else
            pyra.feat_ap{k} = reshape(vocab(:,i)',[sz]);
        end
    end
end

function res = pow_2(in);
res = in.*in;	