startdir = fileparts(mfilename('fullpath'));

opt = 1;
if opt
    optstring = ' -O';
    if ~ispc
        optstring = [optstring ' CXXOPTIMFLAGS="-O3 -DNDEBUG"'];
        optstring = [optstring ' LDOPTIMFLAGS="-O3"'];
    end
else
    optstring = ' -g';
end

if ~ispc
    % Turn all warnings on
    optstringw = [optstring  ' CXXFLAGS="\$CXXFLAGS -Wall"'];
    optstringw = [optstringw ' LDFLAGS="\$LDFLAGS -Wall"'];
else
    optstringw = optstring;
end


%% original GDT code
cd(fullfile(startdir,'voc-release4'))
mexcmd  = ['mex',optstringw];
eval([mexcmd,' dt.cc'])
eval([mexcmd,' features.cc'])
eval([mexcmd,' resize.cc'])
eval([mexcmd,' getdetections.cc'])
eval([mexcmd,' fconv.cc']);
if ~ispc
    eval([mexcmd,' fconvsse.cc']);
end
%% cascade code
cd star-cascade
eval([mexcmd ' cascade.cc model.cc']);
eval([mexcmd ' fconv_var_dim.cc']);



%% DTBB/Lookup binaries
%% compile code for KD-Trees & BB/CD
cd(fullfile(startdir,'cpp'));
outstr = ' -outdir ../mex/';
mexcmd = [mexcmd,outstr];
eval([mexcmd,' mex_kdtree.cpp    KdTree.cpp              '])
eval([mexcmd,' mex_multitree.cpp KdTree.cpp MultiTree.cpp']);
eval([mexcmd,' mex_traverse.cpp  KdTree.cpp MultiTree.cpp']);

%% cascade-only: lookup at prescribed coordinates
sparse_def_script
eval([mexcmd,' sparse_lookup.cpp']);

mexcmd  = ['mex',optstring,outstr];

%% unary terms
dense_def_script
pwd_old = pwd;
for svrt = [4:16],
    nb = 8;
    fexec = sprintf(' -DNH%i -DNB%i dense_lookup.cpp -output fast_%ibit_%i',svrt,nb,nb,svrt);
    eval([mexcmd,fexec]);
    fprintf(2,'.');
end
fprintf(2,'\n');


cd(startdir)