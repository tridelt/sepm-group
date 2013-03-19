### Building
```
mkdir build && cd build     # only needed the first time

cmake ..                    # make sure that you are in ./build
make -j 8                   # when doing those
```

### Dependencies

For gccfilter (makes the output of gcc _much_ nicer), a few perl modules are needed:
```
cpan install Regexp::Common
cpan install Getopt::ArgvFile
cpan install Getopt::Long
cpan install Term::ANSIColor
```

Backwards also needs a library to print pretty stack traces:
```
sudo apt-get install binutils-dev
```

Other libraries (Qt, boost, ZeroC ICE):
```
sudo apt-get install libqt4-dev libboost-program-options-dev libzeroc-ice34-dev
```


Please [file an issue](https://github.com/Mononofu/sepm-group/issues/new) if you had to do anything else to get a working build!
