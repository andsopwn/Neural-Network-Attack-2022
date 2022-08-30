function z=read(filename, trNum, trLen) % z의 1번째 열에 i번째 전력 파형로딩
fid = fopen(filename, 'r');
%header = fread(fid, 2, 'int')
%trLen = header(1);
%trNum = header(2);
%trLen = 24000;
%trNum = num;

for i=1:trNum % z의 크기 trLen * trNum
    z(:,i)=fread(fid, trLen, 'float32');
end

fclose(fid);
end

% trace = read_trace('파일 이름);
% plot(trace(:,1))
% hold
% hold on

