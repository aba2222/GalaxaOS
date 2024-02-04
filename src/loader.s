.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
#.set FLAGS, 0x00000007 VIDEO_MODE(4)+MEMORY_INFO(2)+PAGE_ALIGN(1).equ MULTIBOOT_HEADER_CHECKSUM,-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) & 0xFFFFFFFF.align
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern _kernelMain
.extern _callConstructors
.global loader

loader:
  mov $kernel_stack, %esp

  call _callConstructors
  push %eax
  push %ebx
  call _kernelMain

_stop:
  cli
  hlt
  jmp _stop

.section .bss

.space 2*1024*1024
kernel_stack:
