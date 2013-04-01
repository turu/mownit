plot 'out3gsllag.txt' using 1:2 title 'polynomial' with lines linewidth 3 linecolor rgb "blue",'out3mylag.txt' using 1:2 title 'lagrange' with lines linewidth 2 linecolor rgb "black", 'out3mynew.txt' using 1:2 title 'newton' with lines linecolor rgb "red", 'test3pts.txt' using 1:2 title 'nodes' with points linecolor rgb "green", 'out3gslspl.txt' using 1:2 title 'cspline' with lines linecolor rgb "orange", 'out3gslakm.txt' using 1:2 title 'akima' with lines linecolor rgb "brown"
pause -1
set terminal svg
set output "output.svg"
replot
