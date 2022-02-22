set terminal png



set output "AvgThroughputVsNumFlows.png"
set title "Throughput(Kbps) Vs Number of Flows"
set xlabel "Number of Flows"
set ylabel "Throughput(Kbps)"
plot "AvgThroughputFileVsNumFlows" using 1:2 with lines title "Throughput(Kbps)" lc "blue"



set output "AvgThroughputPerflowVsNumFlows.png"
set title "Throughput(Kbps) Per Flow  Vs Number of Flows"
set xlabel "Number of Flows"
set ylabel "Throughput(Kbps)"
plot "AvgThroughputPerflowFileVsNumFlows" using 1:2 with lines title "Throughput(Kbps)" lc "blue"



set output "DeliveryRatioVsNumFlows.png"
set title "Delivery Ratio(%)  Vs Number of Flows"
set xlabel "Number of Flows "
set ylabel "Delivery Ratio(%)"
plot "DeliveryRatioFileVsNumFlows" using 1:2 with lines title "Delivery Ratio(%)" lc "blue"

set output "DropRatioVsNumFlows.png"
set title "Drop Ratio(%)  Vs Number of Flows"
set xlabel "Number of Flows"
set ylabel "Drop Ratio(%)"
plot "DropRatioFileVsNumFlows" using 1:2 with lines title "Drop Ratio(%)" lc "blue"

set output "DelayVsNumFlows.png"
set title "Delay(ns)  Vs Number of Flows"
set xlabel "Number of Flows"
set ylabel "Delay"
plot "DelayFileVsNumFlows" using 1:2 with lines title "Delay" lc "blue"


