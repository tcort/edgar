# edgar
[![Build Status](https://api.travis-ci.org/tcort/edgar.png?branch=master)](http://travis-ci.org/tcort/edgar)

edgar is a small LISP Interpreter written in C. It doesn't have a lot of
built-in functions yet, but it is already quite functional. It allows users
to define functions and supports some of the classic introduction to LISP
programs like factorial, fibonacci, GCD, LCM, and more. 

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [GNU MP](http://gmplib.org/)
* [valgrind](http://valgrind.org/) (optional - adds memory error/leak checks)
* [cppcheck](http://cppcheck.sourceforge.net/) (optional - adds static analysis checks)
* [wine](http://www.winehq.org/) (optional - runs tests on mingw32 cross builds)

## Building

Standard autotools build (`make check` runs the test suite):

    $ ./autogen
    $ ./configure --prefix=/usr
    $ make
    $ make check
    # make install

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

