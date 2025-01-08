# Budgurse - Budgeting with Curses

A text-only application for tracking and categorizing personal finances with a
curses interface and sensible vi-like key bindings.

![Budgurse Demo Recording](https://raw.githubusercontent.com/Will-C-Aitken/budgurse/master/img/demo.gif)

## Features

- Adding, browsing, and editing spending/earning entries
- Complete control over categories and subcategories
- Summary window with spending per month per category

## Requirements

Any POSIX-compliant OS with:

- autoconf
- automake
- `ncurses` w/ dev files
- `sqlite3` w/ dev files

## Installing

`cd` to budgurse root dir and run the following:

```
$ autoreconf -i
$ ./configure
$ make
# make install
```

## Development

This is my first software project intended for a user-base wider than just
myself. Issue reporting and pull requests are more than welcome and I'm very
open to feedback!

For non-GUI functionality, I've used test-driven development and those unit
tests can be run with `make check`. Contributions ought to add to these tests.
I will make a more comprehensive pull request checklist in the future.


### Project Status

I still consider this project to be in beta (hence the current release being
tagged as a `0.*` version) until I implement the following core functionality:

- Notes - Are currently prompted for but cannot be longer than the window width
    nor are viewable from the browser.
- Handle console resizing - For the time being I just close the program and open
    it again which reinitializes the windows
- Handle large amounts - As sums grow, they may surpass what can fit in a cell 
    in the summary window. I want to switch to $x.xxxK and
    $x.xxxM formats for thousands and millions respectively

Once minimum functionality has been met, the following are features I want to
and/or think could be valuable if implemented, in order of importance:

### Feature Roadmap

- Robust summary:
    - More time delineations (e.g. week, year)
    - Averages/trends
    - Limiting/Extending context
- Smart prompt entry (requires writing a custom parser):
    - Autocomplete entry descriptor
    - Autocomplete categories
    - Dynamic cancellations
- Graphs
- Tertiary categories and so on (could be viewed by selecting entry)
- Non-interactive mode + cli options
- Config file support
- Add `ncursesw` support
- Colours

