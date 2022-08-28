function z=readf(filename, trNum, trLen)
fid = fopen(filename, 'r');

for i=1:trNum 
    z(:,i)=fread(fid, trLen, 'float');
end

fclose(fid);
end

