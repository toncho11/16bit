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

PUBLIC init

EXTRN       _bscrnsav:NEAR
EXTRN       new9:NEAR
EXTRN       new1C:NEAR
EXTRN       start:NEAR

EXTRN       _scrnbuf
EXTRN       maxcount:WORD
EXTRN       count:WORD
EXTRN       old9_ofs:WORD
EXTRN       old9_seg:WORD
EXTRN       old1C_ofs:WORD
EXTRN       old1C_seg:WORD
EXTRN       ourstkseg:WORD
EXTRN       off_flg:BYTE

;------------------------------------------------------------------------------

init_junk:

load_msg    db 13,10,"Falling letters saver resident, v1.0"
	        db " by DL Marks,",13,10,"based on dscrnsav by David Markovich and Peter Summers",13,10
            db " SYNTAX: fallscr [delay], 0 disables,"
            db " 1-9 re-enables, default=5",13,10,10,"$"
dsabl_msg   db 13,10,"fallscr disabled",13,10,"$"
enabl_msg   db 13,10,"fallscr enabled",13,10,"$"
err1_msg    db 13,10,"Invalid command line parameter",13,10,"$"
err2_msg    db 13,10,"fallscr not resident",13,10,"$"

init:                      ; initialization
  mov si,81h
  mov dx,offset err1_msg   ; "Invalid command line parameter"
parse_loop:                ; check for number on command line
  lodsb
  cmp al," "
  je parse_loop            ; skip spaces
  cmp al,13
  je end_parse             ; jump if no parameter
  cmp al,"9"
  jg out_err               ; invalid parameter
  cmp al,"0"
  jl out_err               ; invalid parameter
  cmp al,"0"
  jne set_time
  mov [off_flg],1          ; set disable flag
set_time:
  mov ah,0
  sub al,"0"
  mov bx,1093
  mul bx
  mov [maxcount],ax
end_parse:
  cld                      ; search for a previous copy
  mov [word start],0       ; fingerprint
  mov bx,0                 ; bx = start segment
  mov ax,cs                ; ax = current segment
  cmp ax,0A000h 	   ; Assume first load if running in high memory.
  jae no_copies
chk_prev:                  
  inc bx                   ; increment segment
  cmp ax,bx                ; check if equals current segment
  jne next
  mov bx,0A000h 	   ; if yes, move search to high memory
next:
  cmp bx,0FFFFh 	   ; end search if end of memory reached
  je no_copies
  mov es,bx
  mov si,offset start
  mov di,si
  mov cx,16                ; compare first 16 bytes of each segment
  repe cmpsb
  jne chk_prev             ; repeat if no match

  mov ax,[maxcount]        ; copy found - set time for activation
  mov [es: maxcount],ax
  mov al,[off_flg]         ; change enable/disable flag
  mov [es: off_flg],al
  mov [es: count],0        ; reset tick count
  mov dx,offset enabl_msg  ; "fallscr enabled"
  cmp al,0
  je out_msg               ; copy enabled - errorlevel = 0

  mov dx,offset dsabl_msg  ; "fallscr disabled"
  mov al,0                 ; errorlevel = 0
  jmp out_msg              ; copy disabled

no_copies:
  mov dx,offset err2_msg   ; "fallscr not resident"
  cmp [off_flg],0  
  je install               ; ok to install
  
out_err:
  mov al,1
out_msg:
  mov ah,9                 ; display message
  int 21h
  mov ah,4Ch               ; terminate with errorlevel = al
  int 21h

install:                   ; make program resident
  mov ax,cs
  mov [ourstkseg],ax       ; store our segment in ax
  mov ax,3509H             ; get old int 9 address
  int 21H
  mov ax,es
  mov [old9_seg],ax        ; save old int 9 address  (keyboard)
  mov [old9_ofs],bx
  mov ax,351CH             ; get old int 1C address  (timer tick)
  int 21H
  mov ax,es
  mov [old1C_seg],ax       ; save old int 1C address
  mov [old1C_ofs],bx
  mov dx,offset new9       ; set int 9 to point to our code
  mov ax,2509H
  int 21H
  mov dx,offset new1C      ; set int 1C to point to our code
  mov ax,251CH
  int 21H
  mov dx,offset load_msg   ; "fallscr loaded"
  mov ah,9
  int 21h                  ; display message
  mov dx,offset init_junk
  sub dx,offset start
  add dx,256+16
  shr dx,4
  mov ax,3100H             ; terminate and stay resident
  int 21H

ends
end 

