%%
%% script that generates c++ header for fast lookup-based convolution
%% the filter size is presumed to be 6x6 - this script allows to write the code
%% 1) without a for-loop (faster execution)
%% 2) without writing the same thing 36 times
%% 

rt      = fileparts(mfilename('fullpath'));
f_out  = fopen(fullfile(rt,'sparse_def.h'),'w');
%fprintf(f_out,'// automatically generated header file for fast lookup-based convolution \\\n');
%fprintf(f_out,'// please look into part_def_script.m if you want to modify this header file \\\n');

%% the offsets of consecutive lookup locations
fprintf(f_out,'#define setup_codepos_and_offsets \\\n');
cnt = 0;
for h = 1:6,
    for v = 1:6
        fprintf(f_out,'const int offsets%i =  %i*sv + %i;\\\n',cnt,h-1,v-1);
        cnt = cnt + 1;
    end
end
for cnt = 0:35,
   fprintf(f_out,'const float* values%i  = values_for_models +  %i*Nwords;\\\n',cnt,cnt);
end

%% the image locations  from which lookup operations 'tap' codebook + distance entries
 fprintf(f_out,'\n');
fprintf(f_out,'#define define_taps \\\n');
for cnt = 0:35
    fprintf(f_out,'const pidx* tap%i;\\\n',cnt);
end
for cnt = 0:35
    fprintf(f_out,'const float* dtap%i;\\\n',cnt);
end
for cnt = 0:35
    fprintf(f_out,'const float ntap%i = nrm_loc[%i];\\\n',cnt,cnt);
end

%% slide taps by one
fprintf(f_out,'\n');
fprintf(f_out,'#define update_taps \\\n');
for cnt = 0:35
   fprintf(f_out,'tap%i = pix_offset + offsets%i;\\\n',cnt,cnt);
end
for cnt = 0:35
   fprintf(f_out,'dtap%i = dst_offset + offsets%i;\\\n',cnt,cnt);
end
fprintf(f_out,'\n');

%% add the lookup-based scores
fprintf(f_out,'#define get_sum \\\n');
for cnt = 0:34
    fprintf(f_out,'values%i[*(tap%i++)] + ',cnt,cnt);
end
fprintf(f_out,'values%i[*(tap%i++)]\n',35,35);
fprintf(f_out,'\n');
fprintf(f_out,'#define get_dst_sum \\\n');
for cnt = 0:34
    fprintf(f_out,'*(dtap%i++)*ntap%i + ',cnt,cnt);
end
fprintf(f_out,'*(dtap%i++)*ntap%i\n',35,35);
fclose(f_out);

