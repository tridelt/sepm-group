### Building
```
mkdir build && cd build
cmake ..
make -j 8
```

### Dependencies

For gccfilter (makes the output of gcc _much_ nicer), a few perl modules are needed:
```
cpan install Regexp::Common
cpan install Getopt::ArgvFile
```

Backwards also needs a library to print pretty stack traces:
```
sudo aptitude install binutils-dev
```
