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
<!-- MORE INFORMATION HERE? -->

## Installation

Clone this project and build with command `make`:

```
$ make
```

To install `cp-tools`, use the command below as a super user:

```
# make install
```

The binary will be copied in `/usr/local/bin`. The support files will be copied in `/usr/local/lib/cp-tools` directory.

To uninstall run

```
# make uninstall
```

## Dependencies

- Command line tools: `diff`, `mkdir`, `test`, `cp`, `rm`
- pdflatex
- GCC 8
- Python 3

## Features

<!-- FEATURES THAT ARE ALREADY PRESENT -->
- [x] Integration with Polygon
- [x] Integration with MOJ
- [x] Support Markdown and LaTex
<!-- FUTURE FEATURES -->
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

For more information, use the option `-h` (or `--help`), as shown below:

```
$ cp-tools -h
```
## Tests

<!-- HOW TO RUN THE TESTS? -->

## Technologies

* C++ 17
* GCC v.10.2.0
<!-- THERE ARE MORE Technologies? -->