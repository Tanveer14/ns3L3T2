set terminal png


% set output "AvgThroughputVsNumPackets.png"
% set title "Average Throughput"
% set xlabel "Packets Per Second"
% set ylabel "Average Throughput"
% plot "AvgThroughputFileVsNumPackets" using 1:2 with lines title "Average Throughput" lc "blue"

% set output "DeliveryRatioVsNumPackets.png"
% set title "Delivery Ratio"
% set xlabel "Packets Per Second"
% set ylabel "Delivery Ratio"
% plot "DeliveryRatioFileVsNumPackets" using 1:2 with lines title "Delivery Ratio" lc "blue"

% set output "DropRatioVsNumPackets.png"
% set title "Drop Ratio"
% set xlabel "Packets Per Second"
% set ylabel "Drop Ratio"
% plot "DropRatioFileVsNumPackets" using 1:2 with lines title "Drop Ratio" lc "blue"

% set output "DelayVsNumPackets.png"
% set title "Delay"
% set xlabel "Packets Per Second"
% set ylabel "Delay"
% plot "DelayFileVsNumPackets" using 1:2 with lines title "Delay" lc "blue"

% Coverage

% set output "AvgThroughputVsCoverage.png"
% set title "Average Throughput"
% set xlabel "Coverage"
% set ylabel "Average Throughput"
% plot "AvgThroughputFileVsCoverage" using 1:2 with lines title "Average Throughput" lc "blue"


% set output "DeliveryRatioVsCoverage.png"
% set title "Delivery Ratio"
% set xlabel "Coverage"
% set ylabel "Delivery Ratio"
% plot "DeliveryRatioFileVsCoverage" using 1:2 with lines title "Delivery Ratio" lc "blue"

% set output "DropRatioVsCoverage.png"
% set title "Drop Ratio"
% set xlabel "Coverage"
% set ylabel "Drop Ratio"
% plot "DropRatioFileVsCoverage" using 1:2 with lines title "Drop Ratio" lc "blue"

% set output "DelayVsCoverage.png"
% set title "Delay"
% set xlabel "Coverage"
% set ylabel "Delay"
% plot "DelayFileVsCoverage" using 1:2 with lines title "Delay" lc "blue"


% % Flows

% set output "AvgThroughputVsNumFlows.png"
% set title "Average Throughput"
% set xlabel "Number of Flows"
% set ylabel "Average Throughput"
% plot "AvgThroughputFileVsNumFlows" using 1:2 with lines title "Average Throughput" lc "blue"



% set output "DeliveryRatioVsNumFlows.png"
% set title "Delivery Ratio"
% set xlabel "Number of Flows"
% set ylabel "Delivery Ratio"
% plot "DeliveryRatioFileVsNumFlows" using 1:2 with lines title "Delivery Ratio" lc "blue"

% set output "DropRatioVsNumFlows.png"
% set title "Drop Ratio"
% set xlabel "Number of Flows"
% set ylabel "Drop Ratio"
% plot "DropRatioFileVsNumFlows" using 1:2 with lines title "Drop Ratio" lc "blue"

% set output "DelayVsNumFlows.png"
% set title "Delay"
% set xlabel "Number of Flows"
% set ylabel "Delay"
% plot "DelayFileVsNumFlows" using 1:2 with lines title "Delay" lc "blue"


% % Nodes


% set output "AvgThroughputVsNumNodes.png"
% set title "Average Throughput"
% set xlabel "Number of Nodes"
% set ylabel "Average Throughput"
% plot "AvgThroughputFileVsNumNodes" using 1:2 with lines title "Average Throughput" lc "blue"


% set output "DeliveryRatioVsNumNodes.png"
% set title "Delivery Ratio"
% set xlabel "Number of Nodes"
% set ylabel "Delivery Ratio"
% plot "DeliveryRatioFileVsNumNodes" using 1:2 with lines title "Delivery Ratio" lc "blue"

% set output "DropRatioVsNumNodes.png"
% set title "Drop Ratio"
% set xlabel "Number of Nodes"
% set ylabel "Drop Ratio"
% plot "DropRatioFileVsNumNodes" using 1:2 with lines title "Drop Ratio" lc "blue"

% set output "DelayVsNumNodes.png"
% set title "Delay"
% set xlabel "Number of Nodes"
% set ylabel "Delay"
% plot "DelayFileVsNumNodes" using 1:2 with lines title "Delay" lc "blue"
