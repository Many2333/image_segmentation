%%
%% script that generates c++ header for fast lookup-based convolution
%% works with all root filters - this script allows to write the code
%% 1) without a for-loop (faster execution)
%% 2) without writing the same piece of code multiple times
%% 
rt  = fileparts(mfilename('fullpath'));
df  = fopen(fullfile(rt,'dense_def.h'),'w');
%fprintf(df,'// automatically generated header file for fast lookup-based convolution \\\n');
%fprintf(df,'// please look into root_def_script.m if you want to modify this header file \\\n');

for it =[1,2,4:16]
    fprintf(df,'#ifdef NH%i \n',it);
    fprintf(df,'#define code_nval \\\n');
    % `it' is the vertical dimension of the part filter (Nv).
    for t =[0:it-1],
        fprintf(df,'cf *values%i 	 = values_for_models + Nmodels_all*(*(pix_loc + off%i)) 	 + NmNw%i;\\',t,t,t);
        %if t<it-1,
        fprintf(df,'\n');
        %end
    end
    fprintf(df,'\n');
    fprintf(df,'#define sum_nval');
    for t  = [0:it-2]
        fprintf(df,' values%i[s] +',t);
    end
    fprintf(df,' values%i[s]  \n',it-1);
    fprintf(df,'\n');
    
    fprintf(df,'#define setup_offsets \\\n');
    for t  = [0:it-1]
        fprintf(df,'const int off%i = %i*sv;\\\n',t,t);
    end
    fprintf(df,'\n');
    
    fprintf(df,'#define setup_distortion \\\n');
    for t  = [0:it-1]
        fprintf(df,'cf *nrm%i = nrm_loc + Nma%i;\\\n',t,t);
    end
    fprintf(df,'\n');
    
    fprintf(df,'#define update_distortion_taps \\\n');
     for t  = [0:it-1]
        fprintf(df,'cf dst%i = *(dix_loc + off%i);\\\n',t,t);
     end
    fprintf(df,'\n');
     
    fprintf(df,'#define sum_distortion');
    ch = 1;
    switch ch
        case 1
        for t  = [0:it-2]
            fprintf(df,' nrm%i[s]*(dst%i) +',t,t);
        end
        fprintf(df,' nrm%i[s]*(dst%i) \n',it-1,it-1);
        case 2
            for t  = [0:it-2]
                fprintf(df,' *(dst%i++) +',t);
            end
            fprintf(df,' *(dst%i++) \n',it-1);
        case 3
        for t  = [0:it-2]
            fprintf(df,' nrm%i[s] +',t);
        end
        fprintf(df,' nrm%i[s] \n',it-1);
         case 4
            fprintf(df,' (dst%i[0]) \n',1);
    end
    fprintf(df,'\n');
    if 0,
        fprintf(df,'\n');
        fprintf(df,'#define sum_prod_nval');
        for t  = [0:it-2]
            fprintf(df,'facts[s]*values%i[s] +',t);
        end
        fprintf(df,' facts[s]*values%i[s]  \n',it-1);
        % fprintf(df,'#endif \n');
        fprintf(df,'\n');
    end
    fprintf(df,'#endif \n');
    fprintf(df,'\n');
    
end
fprintf(df,'#define code_setup \\\n');
for it = [0:15]
    fprintf(df,'const int NmNw%i = %i*NmNw;\\\n',it,it);
    fprintf(df,'const int Nma%i  = %i*Nmodels_all;\\\n',it,it);
end
fprintf(df,'\n');
fclose(df);


