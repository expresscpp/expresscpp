# Contributing

We welcome contributions in several forms, e.g.

* Documenting
* Testing
* Coding
* etc.

Please read [14 Ways to Contribute to Open Source without Being a Programming Genius or a Rock Star](http://blog.smartbear.com/programming/14-ways-to-contribute-to-open-source-without-being-a-programming-genius-or-a-rock-star/)

## Git Guidelines

### Workflow

We currently recommend the [Feature Branch Workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow).

The mentioned links from Atlassian are the recommended docs to read and understand the git workflows.

### Git Commit

The cardinal rule for creating good commits is to ensure there is only one "logical change" per commit. There are many reasons why this is an important rule:

* The smaller the amount of code being changed, the quicker & easier it is to review & identify potential flaws.
* If a change is found to be flawed later, it may be necessary to revert the broken commit. This is much easier to do if there are not other unrelated code changes entangled with the original commit.
* When troubleshooting problems using Git's bisect capability, small well defined changes will aid in isolating exactly where the code problem was introduced.
* When browsing history using Git annotate/blame, small well defined changes also aid in isolating exactly where & why a piece of code came from.

Things to avoid when creating commits

* Mixing whitespace changes with functional code changes.
* Mixing two unrelated functional changes.
* Sending large new features in a single giant commit.

### Git Commit Conventions

We use git commit as per (Conventional Changelog)[https://github.com/ajoslin/conventional-changelog]:

    <type>(<scope>): <subject>

Example:

    docs(CONTRIBUTING.md): add commit message guidelines

Allowed types:

* **feat**: A new feature
* **fix**: A bug fix
* **ci**: A ci feature
* **build**: A build feature
* **docs**: Documentation only changes
* **style**: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
* **refactor**: A code change that neither fixes a bug or adds a feature
* **perf**: A code change that improves performance
* **test**: Adding missing tests
* **revert**: Revert some changes
* **chore**: Changes to the build process or auxiliary tools and libraries such as documentation generation

## Code style

```sh
./format-code.sh
```

* C++ is based on the google code style for naming (https://google.github.io/styleguide/cppguide.html) and formatting (see .clang-format)
* CMake we format the cmake files using cmake-format (https://pypi.org/project/cmake-format/)
