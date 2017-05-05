function out = reshape_pyra(pyra_feat)
sv  = size(pyra_feat);
out = single(reshape(pyra_feat,[sv(1)*sv(2),sv(3)]));
