; constants for multiboot header
MBALIGN     equ  1<<0
MEMINFO     equ  1<<1
BOOTDEVICE  equ  1<<1
FLAGS       equ  MBALIGN | MEMINFO  | BOOTDEVICE
MAGIC       equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

; set multiboot section
section .multiboot
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .data
    align 4096

; initial stack
section .initial_stack, nobits
    align 4

stack_bottom:
    ; 1 MB of uninitialized data for stack
    resb 104856
stack_top:

; kernel entry, main text section
section .text
    global _start
    extern kmain


; define _start, aligned by linker.ld script
_start:
    ; set stack
    mov esp, stack_top
    ; reset flags
    push 0
    popf
    ; push multiboot info
    push ebx
    push eax
    call kmain
system_halt:
    jmp system_halt

