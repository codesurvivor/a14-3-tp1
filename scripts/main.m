function [] = main()

%% Display statics following fifo depth and stream period.

fprintf('\nDisplay statics following fifo depth and stream period.\n\n');

for mode=0:4
    display_stat_for_mode(4,4,mode,100000);
end

pause;

%% Display packet statistics following stream period for LRU mode.

fprintf('\nDisplay packet statistics following stream period for LRU mode.\n');

figure;
for i=1:1:5
    dat = csvread(strcat( ...
        '../data/noc_4_4_8_1_100000_', ...
        int2str(2^i), '_packet_trace.data'));
    
    dat(:,4) = ...
        dat(:,3) - dat(:,2);
    
    s = subplot(2,5,i);
    hist(s, dat(:,4));
    title(s, sprintf('transit time (stream period: %d)', 2^i));
    
    dat(:,4) = ...
        dat(:,3) - dat(:,1);
    
    s = subplot(2,5,5+i);
    hist(s, dat(:,4));
    title(s, sprintf('life time (stream period: %d)', 2^i));
end

end

