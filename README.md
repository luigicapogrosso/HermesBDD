# HermesBDD #

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![codecov](https://codecov.io/gh/luigicapogrosso/HermesBDD/branch/main/graph/badge.svg)](https://codecov.io/gh/luigicapogrosso/HermesBDD)
[![Unix Status](https://github.com/luigicapogrosso/HermesBDD/workflows/Unix/badge.svg)](https://github.com/luigicapogrosso/HermesBDD/actions/workflows/unix.yml)
[![Windows Status](https://github.com/luigicapogrosso/HermesBDD/workflows/Windows/badge.svg)](https://github.com/luigicapogrosso/HermesBDD/actions/workflows/windows.yml)

HermesBDD is a novel multi-core and multi-platform [binary decision diagrams](https://en.wikipedia.org/wiki/Binary_decision_diagram) package focused on high performance and usability. HermesBDD supports a static and dynamic memory management mechanism, the possibility to exploit lock-free hash tables, and a simple parallel implementation of the ITE procedure based on a higher-level wrapper for threads and futures.

For more information, progress reports, and more, please visit the HermesBDD [home page](https://luigicapogrosso.github.io/HermesBDD/).

## Dependencies ##

HermesBDD has the following dependencies:

- **CMake** (mandatory), for compiling;
- **Sphinx** (optional), for documentation generation.

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

To install the library globally from built sources, you must do:

```
$ cmake --build . --target install
```

using `sudo` if you require administrator privileges for a Linux installation. Please note that the installation will build the whole distribution beforehand, hence it is preferable that you first build the other targets without administrator privileges, build the install target.

To find the installed library under Ubuntu, you may need to set the `LD_LIBRARY_PATH` in the `.bashrc` file of your home directory:

```
export LD_LIBRARY_PATH=/usr/local/lib
```

### CMake options ###

Available options are:

| CMake Option | Description                          | Default Value |
| ------------ | ------------------------------------ | ------------- |
| NO_CACHE     | Do not use cache                     | OFF           |
| NO_THREAD    | Do not use thread                    | OFF           |
| NO_DYNMEM    | Do not use dynamic memory allocation | OFF           |
| COVERAGE     | Enable coverage reporting            | OFF           |

## Contribution guidelines ##

HermesBDD is not yet feature-complete, and there are still many interesting things left
for _you_ to do. So, this project welcomes contributions and suggestions. If you would
like to contribute to HermesBDD, or do an internship or thesis, please contact the authors.

## License ##

The software files in this repository are provided under the [MIT License](./LICENSE).


## Authors ##

Luigi Capogrosso, Luca Geretti, Marco Cristani, Franco Fummi, Tiziano Villa

*Department of Computer Science, University of Verona, Italy*

`name.surname@univr.it`
