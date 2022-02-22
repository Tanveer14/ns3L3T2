set terminal png




set output "AvgThroughputVsNumFlows.png"
set title "Average Throughput"
set xlabel "Number of Flows"
set ylabel "Average Throughput"
plot "AvgThroughputFileVsNumFlows" using 1:2 with lines title "Average Throughput" lc "blue"



set output "DeliveryRatioVsNumFlows.png"
set title "Delivery Ratio"
set xlabel "Number of Flows"
set ylabel "Delivery Ratio"
plot "DeliveryRatioFileVsNumFlows" using 1:2 with lines title "Delivery Ratio" lc "blue"

set output "DropRatioVsNumFlows.png"
set title "Drop Ratio"
set xlabel "Number of Flows"
set ylabel "Drop Ratio"
plot "DropRatioFileVsNumFlows" using 1:2 with lines title "Drop Ratio" lc "blue"

set output "DelayVsNumFlows.png"
set title "Delay"
set xlabel "Number of Flows"
set ylabel "Delay"
plot "DelayFileVsNumFlows" using 1:2 with lines title "Delay" lc "blue"


