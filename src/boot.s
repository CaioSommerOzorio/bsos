// From: https://wiki.osdev.org/Bare_Bones#Bootstrap_Assembly

// Declare a multiboot header that marks the program as a kernel.
.section .multiboot
.align 8

header_start:
.long 0xE85250D6  # magic
.long 0           # architecture
.long header_end - header_start
.long -(0XE85250D6 + 0 + (header_end - header_start))

.align 8 # type
.short 0 # flags
.short 0 # size
.long 8

header_end:

// Setting up the stack
.section .bss
.align 16
stack_bottom:
.skip 16384 // 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp

  mov $0xB8000, %edi
  mov $0x1F41, (%edi)

  push %ebx
	call kernel_main

	cli
1:	hlt
	jmp 1b

.size _start, . - _start
