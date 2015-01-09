# libHfill
libHfill is a library which provides a simple interface to output results using custom experimental binning.
The interface is provided in C/C++ and Fortran. The library comes with the kinematical bins of jet data from LHC and Tevatron. libHfill has been used in this study. 

- Program to fill histogram in eta, pT, w
- Requirements: ROOT, g++, gfortran
- Examples in test
- The histograms bins are controlled in src/hfill.cc

## Installation

Download the latest release from https://github.com/scarrazza/libhfill/releases.
Checkout the code from our repository and compile the code using the following procedure: 
```Shell
cd libhfill-x.x.x
make
```
You find some examples in the "test" folder. By the default, the Makefile will create a dynamic library, i.e. libHFILL.so, so remember to export the *.so path to the LD_LIBRARY_PATH before running your program. 
