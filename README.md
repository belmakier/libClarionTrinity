# libpixie {#mainpage}

@tableofcontents

libClarionTrinity is a C++ library designed to help processing data
from the CLARION2 and TRINITY HPGe and charged-particle arrays. It is
designed to interface with libpixie, which handles the raw data format
and event-building. libClarionTrinity processes the libpixie events
into detectors and groups of detectors - doing BGO supression, Clover
addback, requiring both signals from a single GAGG crystal etc. etc.

## Installation

Installation is simple, you'll just need to get the source code
and compile to get going. First clone the repository using

```
git clone https://github.com/belmakier/libClarionTrinity.git
```

Then change directory and make some build directories:

```
cd libpixie;
mkdir lib;
mkdir obj;
```

Change directory again and compile

```
cd src
make
```

The install directories are specified in the Makefile, by default they
are ```$HOME/.local/include``` for headers and ```$HOME/.local/lib```
for dynamic libraries. If these don't exist you'll need to create
them. After that, you can install with

```
make install
```

Make sure the ```$HOME/.local/lib``` directory is in your
```$LD_LIBRARY_PATH```, and you're good to go!

# Developers {#authors}
+ Timothy Gray <graytj@ornl.gov>
