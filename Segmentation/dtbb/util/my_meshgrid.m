function [xx,yy,s] = my_meshgrid(x,y)
xx = single(x(:))';
yy = single(y(:));   
nx = length(xx); ny = length(yy);
s  = [nx,ny];
xx = xx(ones(ny, 1),:);
yy = yy(:,ones(1, nx));
xx = xx(:)';
yy = yy(:)';
