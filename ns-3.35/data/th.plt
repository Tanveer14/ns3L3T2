set terminal png
set output "cwndComparisonMultipleFlow(10)10sec.png"
set title "cwnd"
set xlabel "time"
set ylabel "cwnd"
plot "TcpNewReno-cwnd.data" using 1:2 with lines title "TcpNewReno" lc "blue",\
"TcpWestwood-cwnd.data" using 1:2 with lines title "TcpWestwood" lc "red"
