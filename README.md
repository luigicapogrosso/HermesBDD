# HermesBDD #

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://github.com/luigicapogrosso/HermesBDD/workflows/Continuous%20Integration/badge.svg)](https://github.com/luigicapogrosso/HermesBDD/actions)
[![codecov](https://codecov.io/gh/luigicapogrosso/HermesBDD/branch/main/graph/badge.svg)](https://codecov.io/gh/luigicapogrosso/HermesBDD)

This project is a parallel implementation of [binary decision diagram](https://en.wikipedia.org/wiki/Binary_decision_diagram). It focuses on high performance, usability, and correctness.

## Building ##

The build system is CMake. To build the library from sources in a clean way, it is preferable that you set up a build subdirectory, say:

```
$ mkdir build && cd build
```

Then you can prepare the build environment, choosing a Release build for maximum performance:

```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
```

At this point, if no error arises, you can build with:

```
$ cmake --build .
```

## Author ##

`Luigi Capogrosso`
- PhD at Politecnico di Torino, Italy [luigi.capogrosso@polito.it](mailto:luigi.capogrosso@polito.it)

`Luca Geretti`
- Assistant Professor at University of Verona, Italy [luca.geretti@univr.it](mailto:luca.geretti@univr.it)