% Set up global variables used throughout the code

% setup svm mex for context rescoring (if it's installed)
if exist('./svm_mex601') > 0
    addpath svm_mex601/bin;
    addpath svm_mex601/matlab;
end

% dataset to use
if exist('setVOCyear') == 1
    VOCyear = setVOCyear;
    clear('setVOCyear');
else
    VOCyear = '2007';
end

% directory for caching models, intermediate data, and results
if 0,
    cachedir = fullfile(matlabsavedir,'rc',['pas_',cls,VOCyear],'model','grp','model','cache',['data',filesep]);
    
    if ~exist(cachedir)
        if ispc,
            mkdir(cachedir);
        else
            unix(['mkdir -p ' cachedir]);
        end
        if exist(fullfile(cachedir,'learnlog')) == 0
            if ispc,
                unix(['mkdir -p ' cachedir 'learnlog/']);
            else
                mkdir(fullfile(cachedir,'learnlog'));
            end
        end
    end
    
    % directory for LARGE temporary files created during training
    if ispc,
        tmpdir   = fullfile(matlabsavedir,'VOC','dat',VOCyear);
    else
        tmpdir = ['/var/tmp/rbg/YOURPATH/dat/' VOCyear '/'];
    end
    if exist(tmpdir) == 0
        if ispc,
            mkdir(tmpdir);
        else
            unix(['mkdir -p ' tmpdir]);
        end
    end
end
% should the tmpdir be cleaned after training a model?
cleantmpdir = true;

% directory with PASCAL VOC development kit and dataset
if 1, %ispc,
    dtbb_dir  = fileparts(fileparts(mfilename('fullpath')));
    VOCdevkit = fullfile(dtbb_dir,'VOCdevkit');
else
    VOCdevkit = ['/var/tmp/rbg/VOC' VOCyear '/VOCdevkit/'];
end


% directory for cascade algorithm data
cscdir = [fullfile(dtbb_dir,'voc-release4','star-cascade','data'),filesep];

