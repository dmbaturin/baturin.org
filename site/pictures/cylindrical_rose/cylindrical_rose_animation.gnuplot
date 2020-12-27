set view i,i

set parametric
set format x ""
set format y ""
set format z ""
unset key

set samples 200

splot sin(2*u)*cos(u), sin(2*u)*sin(u), -cos(4*u) linetype rgb "red"

i=i+5
if (i < n) reread

