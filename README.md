# chinet

[![Anaconda-Server Badge](https://anaconda.org/tpeulen/chinet/badges/installer/conda.svg)](https://conda.anaconda.org/tpeulen)
[![Anaconda-Server Version](https://anaconda.org/tpeulen/chinet/badges/version.svg)](https://anaconda.org/tpeulen/chinet)
[![Anaconda-Server Badge](https://anaconda.org/tpeulen/chinet/badges/platforms.svg)](https://anaconda.org/tpeulen/chinet)
[![Linux Build Status](https://travis-ci.org/tpeulen/chinet.svg?branch=master)](https://travis-ci.org/tpeulen/chinet)

## General description

chinet is a C++ library to create global fluorescence models. A global
fluorescence model is a model jointly describing multiple data-sets.
chinet can be used to model the 

graph
parallel execution

database backend 

MongoDB



reactive evaluation of computing nodes   
dataflow

chinet is NOT intended as ready-to-use software for specific application 
purposes.


## Design goals

* Low memory footprint (keep objective large datasets, e.g.  FLIM in memory). 
  Particulary useful for FLIM.
* Platform independent C/C++ library with interfaces for scripting libraries 


## Capabilities

* Fast (IO limited) Reading TTTR files
* Generation / analysis of fluorescence decays
* Time window analysis
* Correlation of time event traces
* Filtering of time event traces to generate instrument response 
functions for fluorescence decays analysis without the need of independent measurements.. 
* Fast photon distribution analysis
* Fast selection of photons from a photon stream

Generation of fluorescence decay histograms chinet outperforms pure numpy and Python based
libraries by a factor of ~40  


## Implementation

Pure pure C/C++ and CUDA based high performance algorithms for real-time and interactive 
analysis of TTTR data.

# Building and Installation

## C++ shared library

The C++ shared library can be installed from source with [cmake](https://cmake.org/):

```console
git clone --recursive https://github.com/tpeulen/chinet.git
mkdir chinet/build; cd chinet/build
cmake ..
sudo make install
```

On Linux you can build and install a package instead (prefered):

## Python bindings
The Python bindings can be either be installed by downloading and 
compiling the source code or by using a precompiled distribution for 
Python anaconda environment.


The following commands can be used to download and compile the source 
code:

```console
git clone --recursive https://github.com/tpeulen/chinet.git
cd chinet
sudo python setup.py install
```

In an [anaconda](https://www.anaconda.com/) environment the library can 
be installed by the following command: 
```console
conda install -c tpeulen chinet
```

For most users the later approach is recommended. Currently, 
pre-compiled packages for the anaconda distribution system are 
available for:

* Windows: Python 2.7, Python 3.7 (x64)
* Linux: Python 2.7, Python 3.7 (x64)
* MacOs: Python 2.7 (x64)

Legacy 32-bit platforms are currently not supported.

# Examples


```python

```
  

# License

chinet is released under the open source MIT license.

