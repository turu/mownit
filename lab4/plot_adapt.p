g(x) = sin(666 * x) + 1
G(x) = x - 1. / 666. * cos(666 * x) + 1. / 666.
set xr [0:1]
set yr [0:2]
plot g(x) with lines, G(x) with lines
pause -1
