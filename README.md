# DDC: Displaced Decay Counter

**By Florian Domingo, Julian Günther, Jong Soo Kim, and Zeren Simon Wang**


[![arXiv](http://img.shields.io/badge/arXiv-2308.07371-B31B1B.svg)](https://arxiv.org/abs/2308.07371)
[![license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/wzeren/Displaced-Decay-Counter/blob/master/LICENSE.md)


## Introduction

We develop a tool Displaced Decay Counter, **DDC**, for estimating the signal-event rates of different LLP models at various LHC far detectors. The input files can be LHEF, HepMC, or a CMND run card for Pythia8. It can be easily generalized to the cases of other collider types and detectors. One can also implement new detector's geometries in a straightforward manner into the code.


## Paper

An accompanying paper of this tool can be accessed at [![arXiv](http://img.shields.io/badge/arXiv-2308.07371-B31B1B.svg)](https://arxiv.org/abs/2308.07371). Any user of the tool is strongly suggested to read the paper carefully first, before starting to test the tool.

## Compilation

The code is written in C++. There is a Makefile. In the first line of Makefile, put in the path to your own Pythia8 and HepMC installation. Pythia8 should have been compiled with a link to zlib, in order to allow for reading *.lhe.gz files. If you do not use zlib, remove "-lz" in the line starting with "LDFLAGS". Run "make -jN" in the terminal to compile, where N is the number of CPU cores you want to use.



## Relevant files for the benchmark models

We provide the input files, Python plotting scripts, the final sensitivity plots, as well as the data points, of the theoretical benchmark models considered in the paper, collectively in the folder "examples".


## Doxygen documentation

An updated Doxygen documentation of the code can be accessed at [![doxygen](https://img.shields.io/badge/Doxygen-808080)](https://wzeren.github.io/Displaced-Decay-Counter).



## Questions and bug reports

If you have any questions, or would like to report any bug, you are welcome to write to any of us:

- Florian Domingo: [domingo@physik.uni-bonn.de](mailto:domingo@physik.uni-bonn.de)
- Julian Günther: [guenther@physik.uni-bonn.de](mailto:guenther@physik.uni-bonn.de)
- Jong Soo Kim: [jongsoo.kim@tu-dortmund.de](mailto:jongsoo.kim@tu-dortmund.de)
- Zeren Simon Wang: [wzs@mx.nthu.edu.tw](mailto:wzs@mx.nthu.edu.tw)
