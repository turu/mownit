k(x) = 7 * x * (cos(13 * x) + sin(x))
K(x) = 7 * sin(x) + 7. / 13. * x * sin(13*x) - 7 * x * cos(x) + 7./169. * cos(13*x) - 7./169.
set xr [0:pi]
set yr [-20:30]
plot k(x) with lines, K(x) with lines
pause -1
