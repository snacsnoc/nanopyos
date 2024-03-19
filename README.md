# NanoPy OS
Embedded Python interpreter in a minimal kernel for arm64

TODO: make this all work


# build toolchain
mkdir -p build/toolchain
cd build


export PREFIX=/PATH/build/toolchain
export TARGET=aarch64-none-elf
## Linux
export CC=/usr/local/bin/gcc-12
export CXX=/usr/local/bin/g++-12
export LD=/usr/local/bin/gcc-12
## Mac
export CC=/opt/homebrew/bin/gcc-12
export CXX=/opt/homebrew/bin/g++-12
export LD=/opt/homebrew/bin/gcc-12



## make binutils for aarch64

./configure --prefix=$PREFIX --target=$TARGET --disable-nls --disable-multilib --disable-werror --with-system-zlib \
--libdir=lib/$TARGET --infodir=info/$TARGET
make
make install

cd ..

# make newlib - extra

wget ftp://sourceware.org/pub/newlib/newlib-4.4.0.20231231.tar.gz
tar xvf newlib
cd newlib
./configure --prefix=$PREFIX --target=$TARGET
make
make install
# note: this installs to $PREFIX/$TARGET

## get gcc source

wget http://ftp.gnu.org/gnu/gcc/gcc-12.3.0/gcc-12.3.0.tar.xz
tar xvzf gcc-12.3.0.tar.xz
cd gcc-12.3.0

## get mpfr, gmp and mpc
bash ./contrib/download_prerequisites

## make gcc out of tree
mkdir gcc-build
cd gcc-build

../configure --prefix=$PREFIX --target=$TARGET --disable-multilib --disable-nls --disable-werror --without-headers --without-isl --enable-languages=c,c++,lto \
--with-as=../../../build/toolchain/bin/aarch64-elf-as  --with-ld=../../../build/toolchain/bin/aarch64-elf-ld --with-system-zlib --enable-lto


make all-gcc
make install-gcc
make all-target-libgcc
make install-target-libgcc

export PATH=$PREFIX/bin:$PATH


## get Python
wget https://www.python.org/ftp/python/3.11.8/Python-3.11.8.tar.xz
cd Python-3.11.8/

## cross compile python for arm64

LDFLAGS="-L${TARGET}/lib" \
CFLAGS="-I${TARGET}/include" \
STRIP=${TARGET}-strip \
LD=${TARGET}-ld \
CC=${TARGET}-gcc \
CXX=${TARGET}-g++ \
./configure --prefix=${TARGET} \
--host=${TARGET} --build=${TARGET} \
--disable-ipv6 --without-ensurepip --disable-test-modules \
--without-doc-strings --enable-optimizations --disable-framework \
--with-build-python=python3.11 \
ac_cv_file__dev_ptmx=no ac_cv_file__dev_ptc=no ac_cv_have_long_long_format=yes


# building the project
TODO: compile the C program with embedded Python into a binary and then embedding the binary directly into the kernel image



See `Makefile`

# Resources
https://code.google.com/archive/p/cleese/source/default/source