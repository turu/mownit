h(x) = 1. / sqrt(1. - x)
H(x) = -2. * sqrt(1. - x) + 2
set xr [0:1]
set yr [0:10]
plot h(x) with lines, H(x) with lines
pause -1
