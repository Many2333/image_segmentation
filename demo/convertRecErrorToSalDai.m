function salValue = convertRecErrorToSalDai(SRRecError,sulabel,r,c,supNum)

salValue=zeros(r,c);


for i=1:supNum
    [x,y] = find(sulabel==i);
    for j = 1: length (x)
        salValue(x(j),y(j)) = SRRecError (i);
    end
end

salValue = (salValue - min(salValue(:)))/(max(salValue(:)) - min(salValue(:)));