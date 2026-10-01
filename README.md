# bsOS
bsOS is a minimal operating system for x86_64 written in C.

Its aim is to serve as a recreational exercise in systems programming.

# Usage
You will have to build your own cross compiler. Further instructions can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler)

Once you have a cross compiler, you can build and run bsOS with the following commands:
```bash
git clone https://github.com/CaioSommerOzorio/bsos
cd bsos
make && make run
```
Make sure you have the `qemu` package installed.

If you can't be bothered building a cross compiler, you can just `make run`.

# Documentation
Each header file inside `src/libc/include` contain code documentation for each function. Additional documentation can also be found within the source code in `src/libc/src` if needed.
