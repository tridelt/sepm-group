### Building
```
mkdir build && cd build     # only needed the first time

cmake ..                    # make sure that you are in ./build
make -j 8                   # when doing those
```

### Dependencies

Libraries (Qt, boost, ZeroC ICE), needed to compile:
```
sudo apt-get install libqt4-dev libboost-program-options-dev libzeroc-ice34-dev
```


Optionally, for gccfilter (makes the output of gcc _much_ nicer), a few perl modules are needed:
```
cpan install Regexp::Common
cpan install Getopt::ArgvFile
cpan install Getopt::Long
cpan install Term::ANSIColor
```


If you want to have pretty stacktraces enabled, backwards needs an extra library:
```
sudo apt-get install binutils-dev
```

To see such a pretty stack trace, try
```
./chat_client --non-sense
```


Please [file an issue](https://github.com/Mononofu/sepm-group/issues/new) if you had to do anything else to get a working build!
