# HermesBDD #

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![codecov](https://codecov.io/gh/luigicapogrosso/HermesBDD/branch/main/graph/badge.svg)](https://codecov.io/gh/luigicapogrosso/HermesBDD)
[![Build Status](https://github.com/luigicapogrosso/HermesBDD/workflows/Continuous%20Integration/badge.svg)](https://github.com/luigicapogrosso/HermesBDD/actions)

This project is a parallel implementation of [binary decision diagram](https://en.wikipedia.org/wiki/Binary_decision_diagram). It focuses on high performance, usability, and correctness. For further information, check out the [documentation page](https://luigicapogrosso.github.io/HermesBDD/).

## Building ##

The build system used is CMake. The library is tested for compilation using gcc (minimum required: 10.2) and clang (minimum required: 11.0). To build the library from sources in a clean way, it is preferable that you set up a build subdirectory, say:

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

### CMake options ###

Available options are:
| CMake Option | Description               | Default Value |
| ------------ | ------------------------- | ------------- |
| NO_CACHE     | Do not use cache          | OFF           |
| NO_THREAD    | Do not use thread         | OFF           |
| COVERAGE     | Enable coverage reporting | OFF           |

## Library performance ##

Our baselines are:
| Problem      | Samples   | USE_CACHE  | USE_THREAD | Time (ms) |
|--------------|-----------|------------|------------|-----------|
| N-queens     | 100       | NO         | NO         | 5319      |

Our speedup achievements:
| Problem      | Samples   | USE_CACHE  | USE_THREAD | Time (ms) | Speedup |
|--------------|-----------|------------|----------- |-----------|---------|
| N-queens     | 100       | YES        | YES        | 616       | 8.6     |

## Contribution guidelines ##

If you would like to contribute to HermesBDD, please contact the authors.

## Authors ##

`Luigi Capogrosso`
- PhD at Politecnico di Torino, Italy [luigi.capogrosso@polito.it](mailto:luigi.capogrosso@polito.it)

`Luca Geretti`
- Assistant Professor at University of Verona, Italy [luca.geretti@univr.it](mailto:luca.geretti@univr.it)