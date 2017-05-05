%save_segments(masks, scores, I, img_name, exp_dir);
function save_segments(segments, scores, I, img_name, exp_dir)
save_dir = [exp_dir 'fullSegs/' img_name '/'];
if(~isdir(save_dir))
    mkdir(save_dir);
else;
    
    save([save_dir img_name '.mat'],'segments','scores');
%for i=1:size(segments,3)
%    title = [num2str(i) '_' num2str(scores(i))]
%    bground = zeros(size(I,1), size(I,2), 3);
%    bground(:, :, 2) = 255;
%    alpha_chann = segments(:, :, i)*0.5;
%    Img = immerge(I, bground, alpha_chann);
%    imshow(Img);
%    fig=gcf;
%    print(sprintf('%s%s',save_dir,title), '-djpeg');
%    %print(sprintf('%stest%.4d',figdir,i),'-djpeg');
%end
end