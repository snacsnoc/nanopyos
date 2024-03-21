# NanoPy OS
NanoPy OS is an experimental project to embed a Python interpreter within a minimal kernel for arm64/i386 architecture. The project explores integrating MicroPython and CPython.

TODO: make this all work


## Building the Toolchain

First, set up the toolchain required for cross-compilation.

```
mkdir -p test-build/toolchain
cd build
```


```bash
export PREFIX=/PATH/build/toolchain
export TARGET=aarch64-elf
export PATH=$PREFIX/bin:$PATH
```
## Linux
```bash
export CC=/usr/local/bin/gcc-12
export CXX=/usr/local/bin/g++-12
export LD=/usr/local/bin/gcc-12
```
## Mac
```bash
export CC=/opt/homebrew/bin/gcc-12
export CXX=/opt/homebrew/bin/g++-12
export LD=/opt/homebrew/bin/gcc-12
```



## Build binutils for aarch64
```bash
wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz
tar xvf binutils-2.42.tar.xz
```

```bash
mkdir build && cd build #build out of tree

../configure --prefix=$PREFIX --target=$TARGET --disable-nls --disable-multilib --disable-werror
make
make install
```





## Get gcc source

```bash
wget http://ftp.gnu.org/gnu/gcc/gcc-12.3.0/gcc-12.3.0.tar.xz
tar xvzf gcc-12.3.0.tar.xz
cd gcc-12.3.0
```

## get mpfr, gmp and mpc
`bash ./contrib/download_prerequisites`

### gmp - extra
```bash
cd gmp
mkdir build && cd build
../configure --prefix=$PREFIX --build=aarch64-apple-darwin23 --host=${TARGET} --target=${TARGET} --disable-shared

make
make install
```

### mpfr - extra
```bash
cd mpfr
mkdir build && cd build
CFLAGS="-I$PREFIX/include" ../configure --prefix=$PREFIX --host=${TARGET} --target=${TARGET} --with-gmp=$PREFIX/lib
make
make install
```

## Build gcc out of tree
```bash
mkdir gcc-build
cd gcc-build

../configure --prefix=${PREFIX} --build=aarch64-apple-darwin23 --target=${TARGET} --disable-shared --disable-multilib --disable-nls --disable-werror --without-headers --without-isl --disable-libsanitizer --enable-languages=c,c++  --with-newlib

make all-gcc
make install-gcc
make all-target-libgcc
make install-target-libgcc
```

## Build newlib

```bash
wget ftp://sourceware.org/pub/newlib/newlib-4.4.0.20231231.tar.gz
tar xvf newlib
cd newlib
./configure --prefix=$PREFIX --target=$TARGET --disable-multilib
make
make install
```
Note: this installs to `$PREFIX/$TARGET`

# Building Python Interpreter



# MicroPython
Clone the MicroPython repository:

```bash
mkdir test-build && test-build
git clone https://github.com/micropython/micropython.git
```

See `Makefile.micropython`
Then: make -f Makefile.micropython
```bash
LDFLAGS="-L/$PREFIX/aarch64-elf/lib"  CC=aarch64-elf-gcc make V=1

```

## booting:
```bash
qemu-system-aarch64 -M virt -kernel kernel.img -display none -serial stdio
```

# cpython

See `Makefile`

Download Python source:
```bash
wget https://www.python.org/ftp/python/3.11.8/Python-3.11.8.tar.xz
cd Python-3.11.8/
```

## Configure and build for cross-compilation
_this mostly doesn't work but only sort of_

```bash
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