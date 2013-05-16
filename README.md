### Building
We are using an out of source build, if you've never done this just follow the instructions below:

```
# in the top-level dir of the project
mkdir build && cd build     # only needed the first time

cmake ..                    # make sure that you are in ./build
make                        # when doing those
```

To make sure all your shiny cores are used for compiling, put this into your .zshrc (or .bashrc, if you haven't seen the light yet):
```
MAKEFLAGS="-j 8"
```
Or just manually specify it every time:
```
make -j 8
```


### Testing

I've set up [Google Test](https://code.google.com/p/googletest/) as a unit testing framework. To run tests, simply do `make test`. Tests are in subdirectories of `./test` (corresponding to the source they are testing), if you want to add some either add them to an existing file (if they fit) or create a new one. If you create a new file, make sure to add it to the right `CMakeLists.txt`.

You can also run only a subset of all tests. This is useful if your changes only affect of subset of the codebase. Use one of `make test_common`, `make test_client`, `make test_server`, `make test_plugins` or `make test_plugin_name`.

For an introduction to Google Test, see the [Primer](https://code.google.com/p/googletest/wiki/Primer), for concrete code check out the [samples](https://code.google.com/p/googletest/wiki/Samples).


### Dependencies

Libraries (Qt, boost, ZeroC ICE), binaries, etc. needed to compile:
```
sudo apt-get install git cmake make gcc-4.6 g++-4.6 build-essential libqt4-dev libqca2-dev libboost-program-options-dev libboost-filesystem-dev libboost-thread-dev libzeroc-ice34-dev zlib1g-dev libsqlite3-dev
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

If you want to generate documentation with `make doc`, first install doxygen:
```
sudo apt-get install doxygen
```

Please [file an issue](https://github.com/Mononofu/sepm-group/issues/new) if you had to do anything else to get a working build!


###  Plugins

To make development easier and more independent, I've added a simple plugin system. Plugins live under `./plugins` and are automatically reloaded on change to make rapid development possible. All plugins have to implement the `Plugin` interface from `common/Plugin.h`, for an example see `plugins/hello_world/hello.cpp`. (The interface is bare bones atm, we need to figure out what plugins should be able to do and how).

These are mainly for the client - on the server, it's probably better to run "bots" as separate processes that communicate using zmq. That could work for the client too - just make sure that the plugins have zmq sockets to get and send information.

### Continuous Integration with Jenkins

You can access the Jenkins server at http://sepm.furidamu.org. To login, simply use your GitHub account. Every push to the main repository on GitHub will automatically trigger a rebuild and a full run of the test-suite.


### Documentation

Documentation is automatically generated with doxygen, so please use doxygen-style comments. You can see the documentation online: http://sepm.furidamu.org/doc/, or generate it locally with `make doc`.
The online documentation is automatically kept up to date by Jenkins, so you can rely on it for development.


### Dashboard

To have an easy overview over the project, a pretty dashboard is available on http://dash.furidamu.org/. It's built using [Dashing](http://shopify.github.com/dashing/), you can contribute using the normal project repository.


### Style Checking
Jenkins runs cppcheck for each commit, the results are visible both in jenkins and in the dashboard. If you are feeling particularly masochistic, run `python cmake/lint.py` to see the full linter output from `cpplint.py`
