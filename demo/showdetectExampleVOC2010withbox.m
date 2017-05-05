%  segmentation for VOC 2010 validation class by class
%  by lingzhengdai
% 




%function showdetectExampleVOC2010

close all
clear all

    addpath('./code/');    
    addpath('./external_code/');
    addpath('./external_code/paraFmex/');
    addpath('./external_code/imrender/vgg/');
    addpath('./external_code/immerge/');
    addpath('./external_code/color_sift/');
    addpath('./external_code/vlfeats/toolbox/kmeans/');
    addpath('./external_code/vlfeats/toolbox/kmeans/');
    addpath('./external_code/vlfeats/toolbox/mex/mexa64/');
    addpath('./external_code/vlfeats/toolbox/mex/mexglx/');
    addpath('./external_code/globalPb/lib/');
    addpath('./external_code/mpi-chi2-v1_5/');  

    
    
    
%%%%%%%%%%%%  原始图像  ;
oldpath=pwd; % 这个命令不用管它
input_Oripath='D:\百度云\我的文档\Code\deeplearning\database\VOCdevkit\VOC2010\JPEGImages';
addpath(input_Oripath);
cd(input_Oripath);                    
Files1 = dir('*.jpg');            %所有图像同一后缀
Files1 = [Files1;dir('*.mat')];
Files1 = [Files1;dir('*.gif')];
Files1 = [Files1;dir('*.tif')];
Files1 = [Files1;dir('*.bmp')];%所有图像同一后缀
cd(oldpath);

%%%%%%%%%%%%%% detection 路径 
input_Oripath2='D:\百度云\我的文档\Code\deeplearning\database\detectiondatabase\detectionvoc-release3.1\detectMat2010validation\';
addpath(input_Oripath2);
cd(input_Oripath2);                    
Files2 = dir('*.jpg');            %所有图像同一后缀
Files2 = [Files2;dir('*.mat')];
Files2 = [Files2;dir('*.gif')];
Files2 = [Files2;dir('*.tif')];
Files2 = [Files2;dir('*.bmp')];%所有图像同一后缀
cd(oldpath);

%%%%%%%%%%%%%  segments hyphtheses 路径
input_Oripath3='D:\百度云\我的文档\Code\deeplearning\database\cpmcdatabase\VOC2010validation\masks2010val';
addpath(input_Oripath3);
cd(input_Oripath3);                    
Files3 = dir('*.jpg');            %所有图像同一后缀
Files3 = [Files3;dir('*.mat')];
Files3 = [Files3;dir('*.gif')];
Files3 = [Files3;dir('*.tif')];
Files3 = [Files3;dir('*.bmp')];%所有图像同一后缀
cd(oldpath);

imagepath='D:\百度云\我的文档\Code\deeplearning\database\VOCdevkit\VOC2010\SegmentationClass';
addpath(imagepath);
cd(imagepath);                    
Files4 = dir('*.jpg');            %所有图像同一后缀
Files4 = [Files4;dir('*.mat')];
Files4 = [Files4;dir('*.gif')];
Files4 = [Files4;dir('*.tif')];
Files4 = [Files4;dir('*.bmp')];%所有图像同一后缀
cd(oldpath);




addpath ([cd '/VOCcode']);


siftimgdir='./siftimg/';% the output path of the saliency map
siftdatadir='./siftdata/';% the superpixel label file path


segresultspath = './VOC2010valSegresult\classbyclassSel4'; % the segmentation results path 
Xpath = './2010valAnnotations/';


%mkdir(imgRoot)
mkdir(siftimgdir);
mkdir(siftimgdir);
%mkdir(imagepath);
mkdir(segresultspath);
mkdir(Xpath);
%D:\百度云\我的文档\Code\deeplearning\database\detectiondatabase\detectionvoc-release3.1\siftimg\1



      


pascal_init;

load('DicitSPM.mat', 'DicitSPM')

AA = zeros(200,1);

for  ii =  1 : 20  % VOCopts.nclasses
    
cls = VOCopts.classes{ii};  
filename = './val.txt';
%Txt = dir([imagepath  '\*_val.' 'txt' ]);
ids = textread(filename,'%s');

%Txtname = textread([imagepath, '\', cls '_val.txt'], '%s');

%rec = VOCreadrecxml(Xpath);
%rec = PASreadrecord(sprintf(VOCopts.annopath,'2009_000067'))

%ids = textread(sprintf('D:\百度云\我的文档\Code\deeplearning\database\VOCdevkit\VOC2010\ImageSets\Segmentation'),'%s');
bbb1 = -0.74;
bbb2 = 0.0;
recErrordetect = 1e+003;
%global mask;

 paramSR.lambda2 = 0;                    
 paramSR.mode    = 2;
 paramSR.lambda  = 0.01;


%%
nn = 1

recErrorSPM = cell(100,1); 
for i = 1 :     length(ids);
  
        im = imread([input_Oripath,'\',ids{i},'.jpg']);
        im2 = imread([imagepath,'\',ids{i},'.png']);
        VOCopts.annopath;
        Xpathr = [Xpath ids{i} '.xml'];
         fid = fopen(Xpathr, 'r');
         
        xml = fread(fid, '*char')';
        fclose(fid);
        x=VOCxml2struct(xml);
        rec = VOC2010valxml(x);
        summask = zeros(size(im,1),size(im,2));
        sumRsaliency = zeros(size(im,1),size(im,2));  
       for objnum = 1: size(rec.objects,2) 

       
      
       if strcmp(rec.objects(objnum).class, cls) == 1  
        
        
%       load([input_Oripath2, cls,'\', ids{i},'.mat']);
        load([input_Oripath3,'\', ids{i},'.mat']);

      segoutnameim = [segresultspath,'\', 'im', '\', cls,'\' ids{i} '.jpg']; 
      segoutnamemask = [segresultspath,'\' 'mask', '\', cls,'\' ids{i} '.jpg'];
      saloutname = [segresultspath, '\', 'saliency', '\', cls, '\' ids{i} '.jpg'];

     % for j = 1 :   size(bbox,1) 
     %  if ~isempty(bbox)  && j <= 2   %&&   (abs(bbox(j,5)-bbox(1,5))<0.1) % &&  bbox(j,5)>bbb1     %    %
     %   bbox(j,5)
        xmin = rec.objects(objnum).bbox(1,1);
        ymin = rec.objects(objnum).bbox(1,2);
        xmax = rec.objects(objnum).bbox(1,3);
        ymax = rec.objects(objnum).bbox(1,4);
        areabounding = (ymax - ymin) * (xmax - xmin);  
        clear A
        A = zeros(size(im,1),size(im,2));
        
        A(ceil(ymin) : ceil(ymax),ceil(xmin) : ceil(xmax)) = 1;
       [sele1,selemasks] = selectthemasks ( A, masks , areabounding,im); 
       SB =  selemasks;
            if sele1>0
             idsi = ids{i};
              [ maskR, Rsaliency] = ReconstructbySparse(im, idsi, A, selemasks);  %, selectmasks, A )
              
         %      imf = subplot_auto_transparent(mask, im);             
             end
%             clear    selemasks;   
            
           % figure; showboxes(im, rec.objects(objnum).bbox(1,:));  
      
         summask = summask + maskR; 
         imwrite(summask,segoutnamemask);
         sumRsaliency =  sumRsaliency +Rsaliency.*A;
         imwrite (sumRsaliency, saloutname);
       nn = nn+1
       end   
       end
         
       %  figure; subplot_auto_transparent(summask, im);
       %  imwrite(summask,segoutnamemask);
       %  imwrite (sumRsaliency, saloutname);
       
     clear siftA;
 end     %      每张图像每类的 多个目标的分割
end
 




