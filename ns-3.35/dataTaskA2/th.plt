set terminal png

set output "AvgThroughputVsNumPackets.png"
set title "Throughput(Kbps) Vs Packets per Second"
set xlabel "Packets Per Second"
set ylabel "Throughput(Kbps)"
plot "AvgThroughputFileVsNumPackets" using 1:2 with lines title "Throughput(Kbps)" lc "blue"

set output "DeliveryRatioVsNumPackets.png"
set title "Delivery Ratio(%) Vs Packets per Second"
set xlabel "Packets Per Second"
set ylabel "Delivery Ratio(%)"
plot "DeliveryRatioFileVsNumPackets" using 1:2 with lines title "Delivery Ratio(%)" lc "blue"

set output "DropRatioVsNumPackets.png"
set title "Drop Ratio(%) Vs Packets per Second"
set xlabel "Packets Per Second"
set ylabel "Drop Ratio(%)"
plot "DropRatioFileVsNumPackets" using 1:2 with lines title "Drop Ratio(%)" lc "blue"

set output "DelayVsNumPackets.png"
set title "Delay(ns) Vs Packets per Second"
set xlabel "Packets Per Second"
set ylabel "Delay"
plot "DelayFileVsNumPackets" using 1:2 with lines title "Delay" lc "blue"