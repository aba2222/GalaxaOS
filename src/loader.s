.set MAGIC, 0x1badb002
#.set FLAGS, (1<<0 | 1<<1)
.set FLAGS, 0x00000007 #VIDEO_MODE(4)+MEMORY_INFO(2)+PAGE_ALIGN(1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

  # aout kludge (unused)
  .long 0,0,0,0,0

  # Video mode
  .long   0       # Linear graphics please?(yes=0 no=1)
  .long   1024       # Preferred width
  .long   768       # Preferred height
  .long   32      # Preferred pixel depth

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
