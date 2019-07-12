#!/usr/bin/env gnuplot
# Daniil Baturin, 2013
# Distributed under CC-Zero

set size square

set samples 500

set terminal pngcairo enhanced font ",14"
set title "x = cos^3(t), y = cos^2(t) + sin(t)"
set out "heart.png"

set key bmargin center horizontal noreverse enhanced box linetype -1 linewidth 1.000

unset border
unset key

set xzeroaxis linetype -1
set yzeroaxis linetype -1

set xtics axis nomirror (-1,1)
set ytics axis nomirror (-1,1)

set style fill transparent solid 0.2 border
set style function filledcurves

set parametric
set trange [0:2*pi]

plot cos(t)**3, cos(t)**2 + sin(t) linetype rgb "red"

