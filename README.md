# NanoPy OS
NanoPy OS is an experimental project to embed a Python interpreter within a minimal kernel for arm64 architecture. The project explores integrating both MicroPython and CPython for more Python features.

TODO: make this all work


## Building the Toolchain

First, set up the toolchain required for cross-compilation:

```
mkdir -p build/toolchain
cd build
```


```
export PREFIX=/PATH/build/toolchain
export TARGET=aarch64-none-elf
```
## Linux
```
export CC=/usr/local/bin/gcc-12
export CXX=/usr/local/bin/g++-12
export LD=/usr/local/bin/gcc-12
```
## Mac
```
export CC=/opt/homebrew/bin/gcc-12
export CXX=/opt/homebrew/bin/g++-12
export LD=/opt/homebrew/bin/gcc-12
```



## Build binutils for aarch64

```
./configure --prefix=$PREFIX --target=$TARGET --disable-nls --disable-multilib --disable-werror --with-system-zlib \
--libdir=lib/$TARGET --infodir=info/$TARGET
make
make install
```



# Build newlib - extra

```
wget ftp://sourceware.org/pub/newlib/newlib-4.4.0.20231231.tar.gz
tar xvf newlib
cd newlib
./configure --prefix=$PREFIX --target=$TARGET
make
make install
```
Note: this installs to `$PREFIX/$TARGET`

## Get gcc source

```
wget http://ftp.gnu.org/gnu/gcc/gcc-12.3.0/gcc-12.3.0.tar.xz
tar xvzf gcc-12.3.0.tar.xz
cd gcc-12.3.0
```

## get mpfr, gmp and mpc
`bash ./contrib/download_prerequisites`

### gmp - extra
```
cd gmp
./configure --prefix=$PREFIX --host=aarch64-elf --disable-shared
make
make install
```

### mpfr - extra
```
cd mpfr
CFLAGS="-I$PREFIX/include" ./configure --prefix=$PREFIX --host=${TARGET} --target=$TARGET --with-gmp=$PREFIX/lib
make
make install
```

## Build gcc out of tree
```
mkdir gcc-build
cd gcc-build

../configure --prefix=$PREFIX --target=$TARGET --disable-multilib --disable-nls --disable-werror --without-headers \ 
--without-isl --disable-libsanitizer --enable-languages=c,c++  


make all-gcc
make install-gcc
make all-target-libgcc
make install-target-libgcc

export PATH=$PREFIX/bin:$PATH
```
# Building Python Interpreter



# MicroPython
Clone the MicroPython repository:
```bash
git clone https://github.com/micropython/micropython.git
```

```
LDFLAGS="-L/$PREFIX/aarch64-elf/lib"  CC=aarch64-elf-gcc make V=1

```

See `Makefile.micropython`

# cpython

See `Makefile`

Download Python source:
```
wget https://www.python.org/ftp/python/3.11.8/Python-3.11.8.tar.xz
cd Python-3.11.8/
```

## Configure and build for cross-compilation
_this mostly doesn't work but only sort of_

```
# config.site-aarch64
ac_cv_buggy_getaddrinfo=no
ac_cv_file__dev_ptmx=yes
ac_cv_file__dev_ptc=no
```

```
CONFIG_SITE=config.site-aarch64
LDFLAGS="-L${TARGET}/lib" \
CFLAGS="-I${TARGET}/include" \
STRIP=${TARGET}-strip \
LD=${TARGET}-ld \
CC=${TARGET}-gcc \
CXX=${TARGET}-g++ \
./configure --prefix=${PREFIX} \
--host=${TARGET} --build=${TARGET} \
--disable-ipv6 --without-ensurepip --disable-test-modules \
--without-doc-strings --enable-optimizations --disable-framework \
--with-build-python=python3.11 \
ac_cv_file__dev_ptmx=no ac_cv_file__dev_ptc=no ac_cv_have_long_long_format=yes
```


### building the project
TODO: compile the C program with embedded Python into a binary and then embedding the binary directly into the kernel image





# Resources
https://code.google.com/archive/p/cleese/source/default/source