arb = 1;
figure;
for i=1:1:6
    i
    dat = csvread(strcat('noc_4_4_',int2str(arb),'_1000000_', ...
        int2str(2^i),'_packet_trace.data'));
    dat(:,4)=dat(:,3)-dat(:,2);
    subplot(2,6,i);
    title(int2str(2^i));
    hist(dat(:,4));
    dat(:,4)=dat(:,3)-dat(:,1);
    subplot(2,6,6+i);
    title(int2str(2^i));
    hist(dat(:,4));
    pause(0.1)
end;