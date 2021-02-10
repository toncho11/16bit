# Makefile for the TU58 Device Driver project - RLA [12-Aug-94]

CC=bcc -c -ms -Z -O -Ol -Oe -zBCODE -zD_TEXT -zTCODE -zR_TEXT
ASM=tasm -mx

.c.obj:
   $(CC) $<

.asm.obj:
   $(ASM) /la $*

all:    fallscr.com mfallscr.com 
  
fallscr.com:   dscrnsav.obj scrnsav.obj dinit.obj
   tlink -t -m -s -n dscrnsav scrnsav dinit, fallscr.com

mfallscr.com:   dscrnsav.obj mscrnsav.obj dinit.obj
   tlink -t -m -s -n dscrnsav mscrnsav dinit, mfallscr.com

clean:
   del *.obj
   del *.map
   del *.lst
 
dscrnsav.obj: dscrnsav.asm
dinit.obj: dinit.asm
scrnsav.obj: scrnsav.c
mscrnsav.obj: scrnsav.c
    $(CC) -DMONOSAVE -o$< scrnsav.c