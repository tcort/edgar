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
* [wine](http://www.winehq.org/) (optional - runs tests on mingw32 cross builds)

## Building

Standard autotools build (`make check` runs the test suite):

    $ autoreconf -i
    $ ./configure --prefix=/usr
    $ make
    $ make check
    # make install

Note: `autoreconf -i` is only needed if there is no configure script.

## Cross Compiling (Target: mingw32)

Is edgar running too fast for you? You can cross compile it and run it in
wine to slow things down a bit. Here's how do it assuming your cross toolchain
is in your path and named i686-pc-mingw32.

The first step is to setup a cross build prefix and install libgmp there.

    mkdir -p $HOME/tmp/xbuild
    cd $HOME/tmp/xbuild
    wget ftp://ftp.gmplib.org/pub/gmp-5.1.2/gmp-5.1.2.tar.bz2
    tar jpvxf gmp-5.1.2.tar.bz2
    cd gmp-5.1.2
    ./configure --host=i686-pc-mingw32 --prefix=$HOME/tmp/xbuild/prefix
    make
    make install
    cd ..

The next step is to cross build edgar.

    tar zvxf edgar-1.0.tar.gz
    cd edgar-1.0
    LDFLAGS="-L$HOME/tmp/xbuild/prefix/lib" CPPFLAGS="-I$HOME/tmp/xbuild/prefix/include" ./configure --host=i686-pc-mingw32 --prefix=$HOME/tmp/xbuild/prefix
    make
    make check
    cp src/edgar.exe $HOME
    cd ~
    wine edgar.exe

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

