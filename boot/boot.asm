[org 0x7C00] ; Offset everything by 0x7C00.
KERNEL_OFFSET	equ 0x1000 ; same as defined in linker
KERNEL_SIZE		equ 0x4000
KERNEL_END		equ KERNEL_OFFSET+KERNEL_SIZE

mov ax, KERNEL_END
cmp ax, 0x7C00
jge kernel_size_error

mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_pm

hlt

%include "boot/print.asm"
%include "boot/boot_sector_disk.asm"
%include "boot/32_bit_gdt.asm"
%include "boot/32_bit_print.asm"
%include "boot/32_bit_switch.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_nl

	mov bx, KERNEL_OFFSET
	mov dh, 32 ; The kenral will be much larger in the future, load 16KiB into memory for now
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

kernel_size_error:
	mov ah, 0x0E
	mov al, 'K'
	int 0x10
	hlt

[bits 32]
BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call clear_screen_pm
	call print_string_pm
	call KERNEL_OFFSET
	mov dx, 0x0604
	mov ax, 0x2000
	out dx, ax

BOOT_DRIVE db 0
MSG_PROT_MODE db "Loaded 32-bit", 0
MSG_LOAD_KERNEL db "Loading kernal", 0

; Fill with 510 zeros minus the size of the previous code
times 446-($-$$) db 0
partab:	db 0x80, 0x00, 0x01, 0x01
		db 0x0C, 0x67, 0xE0, 0x52
		db 0x80, 0x1F, 0x00, 0x00
		db 0x78, 0xB0, 0xE6, 0x00

times 510-($-$$) db 0
; Magic number
dw 0xAA55

