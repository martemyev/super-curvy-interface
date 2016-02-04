#!/bin/bash

./super-curvy-interface nonflat_interface.rho 400 0 6 3; mv nonflat_interface_curvy.bin super_curvy.rho
./super-curvy-interface nonflat_interface.vp 400 0 6 3; mv nonflat_interface_curvy.bin super_curvy.vp
./super-curvy-interface nonflat_interface.vs 400 0 6 3; mv nonflat_interface_curvy.bin super_curvy.vs

bin2vts=${HOME}/projects/bin2vts/build/bin2vts
${bin2vts} -i super_curvy.rho -dim 2 -nx 400 -nz 400 -s 3; mv super_curvy.vts super_curvy_rho.vts
${bin2vts} -i super_curvy.vp -dim 2 -nx 400 -nz 400 -s 3; mv super_curvy.vts super_curvy_vp.vts
${bin2vts} -i super_curvy.vs -dim 2 -nx 400 -nz 400 -s 3; mv super_curvy.vts super_curvy_vs.vts
