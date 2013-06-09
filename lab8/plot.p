set iso 30
set samp 30
unset key
set xrange[0:69]
set yrange[0:69]
unset surf
set style line 1 lt 4 lw .5
set pm3d at s hidden3d 1
splot "output.dat" with lines
