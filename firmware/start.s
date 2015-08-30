.section .text.relocate
.global _relocate
_relocate:
    mov sp, #0x2000
    bl relocate

.section .text.boot

.global _start
_start:
    mov sp, #0x8000
    

    ldr r4, =__bss_start
    ldr r9, =__bss_end
    mov r5, #0x0
    mov r6, #0x0
    mov r7, #0x0
    mov r8, #0x0
    b done_cleaning

keep_cleaning:
    stmia r4!, {r5-r8}

done_cleaning:
    cmp r4, r9
    blo keep_cleaning

    bl main


.global halt
halt:
    wfe
    b halt
 
