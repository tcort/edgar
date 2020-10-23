# edgar

edgar is a small LISP Interpreter written in C. It doesn't have a lot of
built-in functions yet, but it is already quite functional. It allows users
to define functions and supports some of the classic introduction to LISP
programs like factorial, fibonacci, GCD, LCM, and more. 

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [cmake](https://cmake.org/)
* [GNU MP](http://gmplib.org/)

## Building

Standard cmake build (`make test` runs the test suite):

    $ cd bin
    $ cmake ..
    $ make
    $ make test
    # make install

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

