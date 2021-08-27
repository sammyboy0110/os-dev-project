# os-dev-project
Expanding my skills by developing a basic operating system

Requirements
----
To compile you must have a cross-compiler called `i386-elf-gcc` in your system's `$PATH` variable. This cross compiler must be compiled to compile for 1386-elf platforms. You must also compile `binutils' with the same options.

Other requirements to compile are `make` and `NASM`

To run this project you must have `qemu`, and to debug you must have `gdb` compiled for 1386-elf platforms, and it must be called `i386-elf-gdb`

Compiling and Runing
----
To compile simply tpye:
```shell script
$ make
```
To run type:
```shell script
$ make run
```
To debug type:
```shell script
$ make debug
```
You can also run it on a VNC server by running:
```shell script
$ make run
```

