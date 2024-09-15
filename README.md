# NanoPy OS
NanoPy OS is an experimental project to embed a Python interpreter using MicroPython within a minimal kernel for arm64/i386 architecture


See [Releases](https://github.com/snacsnoc/nanopyos/releases/tag/0.1) for booting a compiled kernel.
# Building the Toolchain

First, set up the toolchain required for cross-compilation.

```
mkdir -p test-build/toolchain
```
Build directory for source packages:
```
mkdir build-dir/
```

Toolchain will be installed to `test-build/toolchain`:
```bash
export PREFIX=/PATH/test-build/toolchain
export TARGET=aarch64-elf
export PATH=$PREFIX/bin:$PATH
```
#### Linux
```bash
sudo apt-get install gcc-12 g++-12
export CC=/usr/local/bin/gcc-12
export CXX=/usr/local/bin/g++-12
export LD=/usr/local/bin/gcc-12
```
#### Mac
```bash
brew install gcc@12
export CC=/opt/homebrew/bin/gcc-12
export CXX=/opt/homebrew/bin/g++-12
export LD=/opt/homebrew/bin/gcc-12
```


Enter the source directory:

```bash
cd build-dir/
```

## Build binutils
```bash
wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz
tar xvf binutils-2.42.tar.xz
```
Build out of tree:

```bash
cd binutils-2.42/
mkdir build && cd build 

../configure --prefix=$PREFIX --target=$TARGET --disable-nls --disable-multilib --disable-werror
make
make install
```





### Build gcc

```bash
wget http://ftp.gnu.org/gnu/gcc/gcc-12.3.0/gcc-12.3.0.tar.xz
tar xvzf gcc-12.3.0.tar.xz
cd gcc-12.3.0
```

### get mpfr, gmp and mpc
`bash ./contrib/download_prerequisites`

### gmp (extra, not necessary to build the toolchain)
```bash
cd gmp
mkdir build && cd build
../configure --prefix=$PREFIX --build=aarch64-apple-darwin23 --host=${TARGET} --target=${TARGET} --disable-shared

make
make install
```

### mpfr (extra, not necessary to build the toolchain)
```bash
cd mpfr
mkdir build && cd build
CFLAGS="-I$PREFIX/include" ../configure --prefix=$PREFIX --host=${TARGET} --target=${TARGET} --with-gmp=$PREFIX/lib
make
make install
```

### Build gcc out of tree
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
tar xvf newlib-4.4.0.20231231.tar.gz 
cd newlib-4.4.0.20231231
./configure --prefix=$PREFIX --target=$TARGET --disable-multilib
make
make install
```
Note: this installs to `$PREFIX/$TARGET`

# Building Python Interpreter



## MicroPython
Clone the MicroPython repository:

```bash
cd test-build
git clone https://github.com/micropython/micropython.git
```

See `Makefile.micropython`

Then: 
`make -f Makefile.micropython all V=1 DEBUG=1 #for gdb`


## Booting:
```bash
qemu-system-aarch64 -nographic -machine virt,virtualization=off -d unimp,int -cpu cortex-a57 -kernel build/firmware.elf  -s -m 8
```
## Debugging:
```
qemu-system-aarch64 -M virt,virtualization=off -cpu cortex-a57 -kernel build/firmware.elf -nographic -d unimp,int -s -S

#attach gdb:
aarch64-elf-gdb build/firmware.elf
(gdb) target remote localhost:1234

```
Simple test build:
`make -f Makefile.simple`

Boot with:

```qemu-system-aarch64 -nographic -machine virt,virtualization=off -d unimp,int -cpu cortex-a57 -kernel simple-build/firmware.elf```





# Resources
https://code.google.com/archive/p/cleese/source/default/source