# GalaxaOS

It's a i386 operating system.

## What it does
GalaxaOS is a simple operating system for own study. It includes basic functionalities such as:
- Simple memory management
- Interrupt handling
- Multitasking
- Basic drivers for keyboard, mouse, VGA, and SVGA
- Simple GUI with desktop and window management
- Some FAT32 support

## Compiling and Running
To compile and run GalaxaOS, follow these steps:

1. Ensure you have the necessary tools installed:
   - `g++` for compiling C++ code
   - `as` for assembling assembly code
   - `ld` for linking
   - `grub-mkrescue` for creating a bootable ISO
   - `qemu-system-i386` for running the OS in a virtual machine

2. Open a terminal and navigate to the GalaxaOS directory.

3. Run the following command to compile the OS:
   ```sh
   make
   ```

4. To create a bootable ISO, run:
   ```sh
   make kernel.iso
   ```

5. To run the OS in QEMU, execute:
   ```sh
   make run
   ```

6. To clean up the build files, use:
   ```sh
   make clean
   ```
