# cp-tools

![Travis](https://travis-ci.org/edsomjr/competitive-problems-tools.svg?branch=master)

Format, test and pack competitive programming problems.

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
