# Gnuplot script for plotting data in file "data.dat"

set terminal png
set output "data.png"
set title "Benchmark Multithreaded Quicksort"

set ylabel "time in S"
set xlabel "threads"

set yrange [0:4]
set xrange [1:8]
plot col=2 "data.dat" using 1:col with lines title columnheader



