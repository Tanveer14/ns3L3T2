set terminal png
set output "CwndComparisonSingleFlowWithModifiedAlgoForMultiflow.png"
set title "Cwnd"
set xlabel "Time(sec)"
set ylabel "Cwnd"
plot "TcpNewReno-10cwnd.data" using 1:2 with lines title "TcpNewReno" lc "blue",\
"TcpWestwood-10cwnd.data" using 1:2 with lines title "TcpWestwood" lc "green",\
"TcpPetra-10cwnd.data" using 1:2 with lines title "TcpPetra" lc "red"

