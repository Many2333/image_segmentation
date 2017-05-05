function s = procid()

d = pwd();
if ispc,
	i = strfind(d, '\');
else
	i = strfind(d, '/');
end
d = d(i(end)+1:end);
s = d;
