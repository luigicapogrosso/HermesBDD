# HermesBDD #

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![codecov](https://codecov.io/gh/luigicapogrosso/HermesBDD/branch/main/graph/badge.svg)](https://codecov.io/gh/luigicapogrosso/HermesBDD)
[![Build Status](https://github.com/luigicapogrosso/HermesBDD/workflows/Continuous%20Integration/badge.svg)](https://github.com/luigicapogrosso/HermesBDD/actions)

HermesBDD is a parallel multi-core, multi-platform library of [Binary Decision Diagrams](https://en.wikipedia.org/wiki/Binary_decision_diagram) written in C++. It focuses on high performance, usability, and correctness.

How did HermesBDD get its name? In Greek mythology, Hermes, son of Zeus and the nymph Maia, was the messenger of the gods. That's because he was as fast as the wind. Hence the name *HermesBDD*, meaning an extremely fast and light BDD library.

For more information, compatibility lists, details, the latest release, progress reports, and more, please visit the HermesBDD [home page](https://luigicapogrosso.github.io/HermesBDD/).

## Dependencies ##

HermesBDD has the following dependencies:

- **CMake**, for compiling;
- **Sphinx**, for documentation generation.

For further information about CMake and Sphinx, check the following [1](https://cmake.org/) and [2](https://www.sphinx-doc.org/en/master/) documentation pages, respectively.

## Building ##

The library is tested for compilation using GCC (minimum required: 10.2), Clang (minimum required: 11.0), and MSVC (minimum required: 19.20). To build the library from sources in a clean way, it is preferable that you set up a build subdirectory, say:

```
$ mkdir build && cd build
```

Then, you can prepare the built environment, choosing a `Release` build for maximum performance:

```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
```

At this point, if no error arises, you can build with:

```
$ cmake --build .
```

### CMake options ###

Available options are:
| CMake Option | Description                          | Default Value |
| ------------ | ------------------------------------ | ------------- |
| NO_CACHE     | Do not use cache                     | OFF           |
| NO_THREAD    | Do not use thread                    | OFF           |
| NO_DYNMEM    | Do not use dynamic memory allocation | OFF           |
| COVERAGE     | Enable coverage reporting            | OFF           |

## Library performance ##

The performance of the library should be comparable to other highly optimized BDD implementations. 
Below are the timings to solve the following problem:

> Given N, then in how many ways can N queens be placed on an N x N chessboard
> without threatening each other?

The times are calculated on a 64-bit AMD Ryzen Threadripper 1950X CPU @ 3.4GHz machine. In particular, our baselines are:

| Problem        | Samples   | USE_CACHE  | USE_THREAD | Core | Time (ms) |
|--------------- |-----------|------------|------------|------|-----------|
| N-queens (7x7) | 50        | NO         | NO         | 2    | 5896      |
| N-queens (7x7) | 50        | NO         | NO         | 8    | 5771      |
| N-queens (7x7) | 50        | NO         | NO         | 16   | 5651      |
| N-queens (7x7) | 50        | NO         | NO         | 32   | 5589      |

| Problem        | Samples   | USE_CACHE  | USE_THREAD | Core | Time (ms) |
|--------------- |-----------|------------|------------|------|-----------|
| N-queens (8x8) | 50        | NO         | NO         | 2    | 91270     |
| N-queens (8x8) | 50        | NO         | NO         | 8    | 86751     |
| N-queens (8x8) | 50        | NO         | NO         | 16   | 85278     |
| N-queens (8x8) | 50        | NO         | NO         | 32   | 84724     |

And, our speedup achievements are:

| Problem        | Samples   | USE_CACHE  | USE_THREAD | Core | Time (ms) | Speedup |
|--------------  |-----------|------------|----------- |------|-----------|---------|
| N-queens (7x7) | 50        | YES        | YES        | 2    | 1687      | 3.49x   |
| N-queens (7x7) | 50        | YES        | YES        | 8    | 1280      | 4.45x   |
| N-queens (7x7) | 50        | YES        | YES        | 16   | 1102      | 5.12x   |
| N-queens (7x7) | 50        | YES        | YES        | 32   | 1004      | 5.57x   |

| Problem        | Samples   | USE_CACHE  | USE_THREAD | Core | Time (ms) | Speedup |
|--------------- |-----------|------------|------------|------|-----------|---------|
| N-queens (8x8) | 50        | YES        | YES        | 2    | 11007     | 8.29x   |
| N-queens (8x8) | 50        | YES        | YES        | 8    | 7080      | 12.25x  |
| N-queens (8x8) | 50        | YES        | YES        | 16   | 6085      | 14.01x  |
| N-queens (8x8) | 50        | YES        | YES        | 32   | 5676      | 14.92x  |


## Contribution guidelines ##

HermesBDD is not yet feature-complete, and there are still many interesting things left 
for _you_ to do. So, this project welcomes contributions and suggestions. If you would 
like to contribute to HermesBDD, please contact the authors.

## License ##
The software files in this repository are provided under the [MIT License](./LICENSE).

## Authors ##

`Luigi Capogrosso`
- PhD at Politecnico di Torino in collaboration with the University of Verona, Italy [luigi.capogrosso@univr.it](mailto:luigi.capogrosso@univr.it)

`Luca Geretti`
- Assistant Professor at University of Verona, Italy [luca.geretti@univr.it](mailto:luca.geretti@univr.it)

## References ##

- [[van2015sylvan](https://link.springer.com/content/pdf/10.1007/978-3-662-46681-0_60.pdf)] van Dijk, T., & van de Pol, J. (2015, April). “_Sylvan: Multi-core decision diagrams._” _In International Conference on Tools and Algorithms for the Construction and Analysis of Systems_ (pp. 677-691). Springer, Berlin, Heidelberg.

- [[Kunkle10](https://dl.acm.org/doi/abs/10.1145/1837210.1837222)] 
Kunkle, D., Slavici, V., & Cooperman, G. (2010, July). “_Parallel disk-based computation for large, monolithic binary decision diagrams._” _In Proceedings of the 4th International Workshop on Parallel and Symbolic Computation_ (pp. 63-72).
