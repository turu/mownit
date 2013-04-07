naive(x) = -47.1735 + 14.5809*x + -0.628246*x**2 + 0.00845857*x**3
trick(x) = -12.4398 + 3.75142*x + -0.0732495*x**2 + 0.00574049*x**3
blas(x) = 6.97314 + -0.634712*x + 0.0292151*x**2 + 0.000656881*x**3
plot 'toplot.txt' using 1:2:3 title 'naive' with yerrorbars, 'toplot.txt' using 1:4:5 title 'trick' with yerrorbars, 'toplot.txt' using 1:6:7 title 'blas' with yerrorbars, naive(x) with lines, trick(x) with lines, blas(x) with lines
pause -1
