set terminal png
set output "throughput.png"
set title "throughput calculation"
set xlabel "time"
set ylabel "throughput"

plot "output" using 1:2 with linespoints title "throughputGraph"