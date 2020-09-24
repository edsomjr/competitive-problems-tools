# CONTRIBUTING

First off, thanks for taking the time to contribute!

Contributions are always welcome, no matter how large or small. Before contributing,
please read the [code of conduct](CODE_OF_CONDUCT.md).

Some thoughts to help you contribute to this project

[How Can I Contribute?](#how-can-i-contribute)
- Reporting Bugs
- Suggesting Enhancements
- Pull Requests

[Styleguides](#styleguides)
  * [Git Commit Messages](#git-commit-messages)
  * [Documentation Styleguide](#documentation-styleguide)


## Code of Conduct
This project and everyone participating in it is governed by the [cp-tools of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to <email@to.report>.

## What should I know before I get started?

This project is a tool to help format marathon programming problems. This tool is called _cp-tools_ and is in its early stages of development. Before you start contributing to the project, make sure you know what programming marathons and problem formatting are all about.

### Design Decisions

When we make a significant decision in how we maintain the project and what we can or cannot support, we will document it in the [LINK PARA DOCUMENTO EXPLICANDO O DESIGN DO PROJETO]. If you have a question around how we do things, check to see if it is documented there. If it is *not* documented there, please open a new topic on [LINK PARA CANAL DE COMUNICAÇÃO](https://maratonaunb.slack.com/) and ask your question.

## Recommended Communication Style

1. Always leave screenshots for visuals changes
1. Always leave a detailed description in the Pull Request. Leave nothing ambiguous for the reviewer.
1. Always review your code first. Do this by leaving comments in your coding noting questions, or interesting things for the reviewer.
1. Always communicate. Whether it is in the issue or the pull request, keeping the lines of communication helps everyone around you.

## Setup (forks are preferred).

Make sure you have all dependencies installed:
- Command line tools: `diff`, `mkdir`, `test`, `cp`, `rm`
- pdflatex
- GCC 8
- Python 3


```sh
$ git clone https://github.com/edsomjr/competitive-problems-tools
$ cd competitive-problems-tools
$ make
$ make install
```

## Testing

```sh
# script to run the tests
$ 
```

 

Also, all questions are welcomed.

## Community

Got Questions? Join the conversation in our [Slack](https://maratonaunb.slack.com/).

## Coding Tips
- Ask questions if you are stuck.  

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature")
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
* Limit the first line to 72 characters or less
* Reference issues and pull requests liberally after the first line

### Issues

If you plan to contribute a change based on an open issue, please assign yourself. Issues that are not assigned are assumed open, and to avoid conflicts, please assign yourself before beginning work on any issues.

If you would like to contribute to the project for the first time, please consider joining checking the [bug](https://github.com/edsomjr/competitive-problems-tools/issues?q=is%3Aissue+is%3Aopen+label%3A%22%F0%9F%90%9B+bug%22) or [good first issue](https://github.com/edsomjr/competitive-problems-tools/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) labels.


### Pull Requests

**_We actively welcome your pull requests, however linking your work to an existing issue is preferred._**

1. Fork the repo and create your branch from `main`.
1. Name your branch something that is descriptive to the work you are doing. i.e. adds-new-thing or fixes-mobile
1. If you've added code that should be tested, add tests.
1. If you've changed APIs, update the documentation.
1. If you make visual changes, screenshots are required.
1. Ensure the test suite passes.
1. Make sure you address any lint warnings.
1. If you make the existing code better, please let us know in your PR description.
1. A PR description and title are required. 
1. [Link to an issue](https://help.github.com/en/github/writing-on-github/autolinked-references-and-urls) in the project. Unsolicited code is welcomed, but an issue is the best place to announce your intentions to work on things.


## License

By contributing to the Competitive Problems Tools project, you agree that your contributions will be licensed
under its [GPL-3.0 License](LICENSE).
