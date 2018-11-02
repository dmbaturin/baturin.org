#!/usr/bin/env gnuplot
# Daniil Baturin, 2014
# Distributed under CC-Zero

set terminal pngcairo enhanced
set view 80,20
set out "heart_3d.png"
set title "x = u, y = cos(u)*cos(v)^3, z = (cos(v)^2 + sin(v))*cos(u)"

# Disable axis ticks
set format x ""
set format y ""
set format z ""

# Disable the legend
unset key
unset colorbox

set hidden3d

set isosamples 50,50

set parametric
set urange [-pi:pi]
splot u, (cos(v)**3)*cos(u)  , (cos(v)**2 + sin(v))*cos(u) ls 1 linetype rgb "red"
