set terminal png



set output "AvgThroughputVsCoverage.png"
set title "Throughput(Kbps) Vs Coverage(m)"
set xlabel "Coverage(m)"
set ylabel "Throughput(Kbps)"
plot "AvgThroughputFileVsCoverage" using 1:2 with lines title "Throughput(Kbps)" lc "blue"


set output "DeliveryRatioVsCoverage.png"
set title "Delivery Ratio(%) Vs Coverage(m)"
set xlabel "Coverage(m)"
set ylabel "Delivery Ratio(%)"
plot "DeliveryRatioFileVsCoverage" using 1:2 with lines title "Delivery Ratio(%)" lc "blue"

set output "DropRatioVsCoverage.png"
set title "Drop Ratio(%) Vs Coverage(m)"
set xlabel "Coverage(m)"
set ylabel "Drop Ratio(%)"
plot "DropRatioFileVsCoverage" using 1:2 with lines title "Drop Ratio(%)" lc "blue"

set output "DelayVsCoverage.png"
set title "Delay(ns) Vs Coverage(m)"
set xlabel "Coverage(m)"
set ylabel "Delay"
plot "DelayFileVsCoverage" using 1:2 with lines title "Delay" lc "blue"
