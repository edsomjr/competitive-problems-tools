# cp-tools

<!-- FUTURE cp-tools LOGO -->
<h1 align="center">
    <img src=".github/static/cpp-logo.png" alt="Competitive programming Tools Logo" width="200">
</h1>

<p align='center'>
    <img src='https://travis-ci.org/edsomjr/competitive-problems-tools.svg?branch=master'>
    <img src='https://img.shields.io/badge/License-GPLv3-blue.svg'>
    <img src='https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square'>
</p>

<p align="center">
  <a href="#about">About the project</a> •
  <a href="#installation">Installation</a> •
  <a href="#features">Features</a> •
  <a href="#basic-usage">Basic usage</a> •
  <a href="#tests">Tests</a> •
  <a href="#technologies">Technologies</a> •
</p>

<br>

## About
Format, test and pack competitive programming problems.

## Installation

Clone this project and build with command `make`:

```
$ make
```

To install `cp-tools`, use the command below as a super user:

```
$ make install
```

The binary will be copied in `/usr/local/bin`. The support files will be copied in `/usr/local/lib/cp-tools` directory.

To uninstall run

```
$ make uninstall
```

## Dependencies

- pdflatex
- GCC 10
- Python 3
- libssl-dev
- clang-format (optional)

## Features

<!-- FEATURES THAT ARE ALREADY PRESENT -->
- [x] Generates the problem PDF and checks the solutions
- [ ] Integration with Polygon
- [ ] Integration with MOJ
- [ ] Support for interactive questions

## Basic usage

To create a basic problem template use the following command on an empty directory:

```
$ cp-tools init
```

To remove auto-generated files (which are located in `.cp-build` folder in current directory), use the command

```
$ cp-tools clean
```

To generate the PDF version of the problem, use the command

```
$ cp-tools genpdf
```

The command

```
$ cp-tools check
```
verifies the validator (flag `-v`), the checker (flag `-c`) or the tests (flag `-t`).

To judge a solution, use the command

```
$ cp-tools judge solution[.c|.cpp|.java|.py]
```

To connect to Polygon API use the command

```
$ cp-tools polygon
```

For more information, use the option `-h` (or `--help`), as shown below:

```
$ cp-tools -h
```

Or read cp-tools man page using

```
$ man cp-tools
```

## Tests

To run the unit test use the commands

```
$ make
$ ./cp-run_tests
```

### Polygon connection tests

On a local run of the tests you need to set both environment variables: `POLYGON_KEY` and `POLYGON_SECRET`.

If you are using Travis on your own fork you must add the environment variables `POLYGON_KEY` and `POLYGON_SECRET` following the [official tutorial](https://docs.travis-ci.com/user/environment-variables/#defining-variables-in-repository-settings).

## Technologies

* C++20
* GCC v.10.3.0
* LaTeX

## Troubleshooting

### Outdated GCC

`g++: error: unrecognized command line option ‘-std=c++20’; did you mean ‘-std=c++2a’?`

If you get this error when running `make`, it's because you are using an outdated version of GCC.

The solution is installing `gcc-10` and `g++-10` packages and updating its alternatives:

```
$ apt -y install gcc-10 g++-10
$ update-alternatives remove-all g++
$ update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 30
```
