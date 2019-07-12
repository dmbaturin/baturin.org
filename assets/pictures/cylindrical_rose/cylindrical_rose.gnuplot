unset key

set parametric

set terminal pngcairo enhanced
set title "x = sin(2*u)*cos(u), y = sin(2*u)*sin(u), z = -cos(4*u)"
set output "cylindrical_rose.png"

set format x ""
set format y ""
set format z ""

set view 60,40

set samples 200


splot sin(2*u)*cos(u), sin(2*u)*sin(u), -cos(4*u) linetype rgb "red"

