; Screen saver C part
;
;  Can be used to simply implement many types of screen savers
;
;  Copyright (C) 2007 by Daniel Marks
;
;  This software is provided 'as-is', without any express or implied
;  warranty.  In no event will the authors be held liable for any damages
;  arising from the use of this software.
;
;  Permission is granted to anyone to use this software for any purpose,
;  including commercial applications, and to alter it and redistribute it
;  freely, subject to the following restrictions
;
;  1. The origin of this software must not be misrepresented; you must not
;     claim that you wrote the original software. If you use this software
;     in a product, an acknowledgment in the product documentation would be
;     appreciated but is not required.
;  2. Altered source versions must be plainly marked as such, and must not be
;     misrepresented as being the original software.
;  3. This notice may not be removed or altered from any source distribution.
;  
;  Daniel L. Marks profdc9@gmail.com

; based on
; Program dscrnsav.asm
; David Markovitch 1/12/90 and Peter Summers 21/9/91

IDEAL

SEGMENT _TEXT PUBLIC 'CODE'
ASSUME CS: _TEXT, DS: _TEXT

EXTRN       _bscrnsav:NEAR
EXTRN       _escrnsav:NEAR
EXTRN       init:NEAR

PUBLIC      _scrnbuf
PUBLIC      _rows
PUBLIC      _cols
PUBLIC      _mono
PUBLIC      _tick
PUBLIC      _first

PUBLIC      count
PUBLIC      maxcount
PUBLIC      old9_ofs
PUBLIC      old9_seg
PUBLIC      old1C_ofs
PUBLIC      old1C_seg
PUBLIC      ourstkseg
PUBLIC      off_flg
PUBLIC      start

PUBLIC      new9
PUBLIC      new1C

org 100h

start:
  jmp init          ; jump to initialization

shft_cnt    equ 37   ; ticks between screen shifts  37 = 2 sec

id          db "FALLSCRN,DMPS"
maxcount    dw 5465  ; timer ticks for delay 5465 = 5 min
old9_ofs    dw ?     ; old int 9 address
old9_seg    dw ?
old1C_ofs   dw ?     ; old int 1C address
old1C_seg   dw ?
old_tail    dw ?     ; keyboard buffer tail
offset_reg  dw ?
off_flg     db 0     ; 1 = disable blanking  0 = normal operation    
scr_offst   dw 0     ; offset for shifting screen
blank       db 0     ; 1 = 'blank' screen  0 = normal screen
turn_on     db 0     ; 1 = unblank screen  0 = nothing
count       dw 0     ; tick counter for blanking
_scrnbuf    dd 0     ; screen buffer to use for blanker
_rows       dw 0
_cols       dw 0
_mono       db 0
_tick       dw 0
_first      db 0
ourstkseg   dw 0              ; our stack segment
ourstkofs   dw offset endstk  ; our stack offset
oldstkseg   dw 0 
oldstkofs   dw 0 
stk         db 256 dup (0)
endstk      dw 0

;------------------------------------------------------------------------------

proc new9                 ; new int 9  (keyboard)
  push es
  push ds
  push ax
  push cs
  pop ds
  mov ax,40h
  mov es,ax
  mov ax,[word es: 1Ch]    ; save keyboard buffer tail pointer   
  mov [old_tail],ax
  pushf                    ; do bios keyboard handler
  call [dword old9_ofs]
  cmp [blank],0            ; check for blank screen
  jne rest_scrn
  mov [count],0            ; reset tick counter
  jmp exit_9

rest_scrn:                 ; set flag to unblank screen
  mov [turn_on],1
  mov ax,[old_tail]        ; restore tail pointer to remove key from buffer
  mov [word es: 1Ch],ax
exit_9:
  pop ax
  pop ds
  pop es
  iret
endp new9

;------------------------------------------------------------------------------

proc new1C                 ; new int 1C  (timer tick)
  cli
  mov  [cs:oldstkseg],ss   ; save stack pointer
  mov  [cs:oldstkofs],sp
  mov  ss,[cs:ourstkseg]   ; go to local stack
  mov  sp,[cs:ourstkofs]
  sti                      ; enable interrupts
  push es
  push ds
  push dx
  push ax
  push cs
  pop ds
  
  inc [_tick]
  mov ax,40h
  mov es,ax
  cmp [turn_on],0          ; check if flag set to restore screen
  je if_blank              ; jump to check for blank screen if flag not set

  PUSH  ES BX CX DI SI BP
  call _escrnsav
  POP   BP SI DI CX BX ES
  
  mov [turn_on],0          ; restore original video buffer start
  mov [blank],0
  cmp [_mono],1
  je abort1C
  mov  ax,[offset_reg]     ; get original register
  call set_crt             ; set the crt register
  jmp exit_1C              ; leave now

if_blank:                  
  cmp [off_flg],0          ; exit if disabled
  jne abort1C
  cmp [blank],0            ; check if screen already blank
  jne run_iter             ; if blank then jump to check if ready to shift 

  inc [count]              ; screen not blank - check if time to blank
  mov ax,[maxcount]
  cmp [count],ax
  jne abort1C              ; exit if not time to blank

  mov al,[es: 49h]         ; is the video mode a text mode (0/1/2/3/7) ?
  cmp al,7h
  je  vidmode
  cmp al,4h
  jl  vidmode
abort1C:
  jmp exit_1C              ; not something we can deal with, no screen saver right now
vidmode:
  shr al,2
  mov [_mono],al
  mov ax,[es: 4Ah]         ; number of columns
  cmp ax,0                 ; if = 0 does not compute, abort!
  je  abort1C
  mov [_cols],ax
;  mov ax,[es: 4Ch]         ; divide the number of characters in display by columns
;  xor dx,dx
;  div [_cols]
;  shr ax,1
;  mov [_rows],ax           ; that's the number of rows
  mov [_rows],25
    
  mov [blank],1            ; set blank screen flag
  mov [_first],1           ; tell screen saver we just went blank
  call get_crt
  mov [offset_reg],ax      ; save offset register
  cmp [_mono],1
  je noadj
  add ax,0800h
noadj:
  push ax
  call set_crt             ; go to next page
  pop  ax
  shl  ax,1
  mov [word ptr _scrnbuf],ax

getseg:
  mov ax,[word es: 63h]    ; get CRT address
  cmp ax,3B4h              ; is it MONO?
  jne is_color
  mov [word ptr _scrnbuf+2],0B000h       ; mono segment
  jmp run_iter
is_color:
  mov [word ptr _scrnbuf+2],0B800h      ; color segment

run_iter:
  PUSH  ES BX CX DI SI BP
  call _bscrnsav
  POP   BP SI DI CX BX ES

exit_1C:
  pop ax
  pop dx
  pop ds
  pop es
  cli                       ; be safe, turn off interrupts
  mov  sp,[cs:oldstkofs]    ; go to old stack ptr
  mov  ss,[cs:oldstkseg]
  sti
  jmp [dword cs: old1C_ofs] 

get_crt:                    ; set crt controller
  mov dx,[es: 63h]         ; crt controller base address
  mov al,12                ; access register 12
  out dx,al
  inc dx
  in  al,dx                ; read hi byte
  xchg ah,al
  push ax
  dec dx
  mov al,13                 ; access register 12
  out dx,al
  inc dx
  pop ax
  in al,dx                 ; read low byte
  ret
  
set_crt:                    ; set crt controller
  push ax
  mov dx,[es: 63h]         ; crt controller base address
  mov al,12                ; access register 12
  out dx,al
  inc dx
  pop ax                   ; pop starting address 
  xchg ah,al
  out dx,al                ; write hi (start address) to register 12
  dec dx
  push ax                  ; push starting address
  mov al,13                ; access register 13
  out dx,al
  inc dx
  pop ax                   ; pop starting address
  xchg ah,al
  out dx,al                ; write lo (start address) to register 13
  ret

endp new1C

ends
end start

