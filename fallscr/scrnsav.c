/* Screen saver C part

  Can be used to simply implement many types of screen savers in MGA or CGA/EGA/VGA text screens

  Copyright (C) 2014 by Daniel Marks

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Daniel L. Marks profdc9@gmail.com

*/

extern unsigned short far *scrnbuf;
extern unsigned int rows;
extern unsigned int cols;
extern unsigned char mono;
extern unsigned char first;
extern unsigned int tick;
  
static unsigned int lfsr = 1;

static unsigned int attrib;
static unsigned int boldattrib;

#ifdef MONOSAVE
#define MAXCOL 80
#else
#define MAXCOL 132
#endif

#define MAXROW 25

#define ATTRIB 0x0200
#define BOLDATTRIB 0x0A00

static int currow[MAXCOL];
static int curlen[MAXCOL];
static unsigned char rate[MAXCOL];
static unsigned int inc[MAXCOL];

#ifdef MONOSAVE
static unsigned short copyframebuf[MAXCOL*MAXROW];
#endif

#define SCREEN_LOC(x,y) scrnbuf[(y)*cols+(x)]
 
static unsigned int random_lfsr(void)
{
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);   
    return lfsr;
}

void escrnsav(void)
{
#ifdef MONOSAVE
    int i, r;
    r=rows*cols;
    for (i=0;i<r;i++) scrnbuf[i] = copyframebuf[i];
#endif
}

#define SETSCREENLOCATION(x,y,c) SCREEN_LOC((x),(y)) = (c)
 
void bscrnsav(void)
{
   int i, r;
   
   if (first)
   {
      first = 0;

      lfsr += tick;
      if (!lfsr) lfsr++;

      if (cols > MAXCOL)
        cols = MAXCOL;
      if (rows > MAXROW)
        rows = MAXROW;
        
      r=rows*cols;
#ifdef MONOSAVE
      for (i=0;i<r;i++)
         copyframebuf[i] = scrnbuf[i];
#endif
      for (i=0;i<r;i++) scrnbuf[i] = (ATTRIB | 0x0020);     
      for (i=0;i<cols;i++)
      {
         currow[i]=random_lfsr() % (2*rows) - rows;
         curlen[i]=random_lfsr() % rows;
         rate[i]=(random_lfsr() % 0xE0)+0x10;
         inc[i] = 0;         
      }
   }
   for (i=0;i<cols;i++)
   {
      inc[i] += rate[i];
      if (inc[i] & 0xFF00)
      {
        currow[i]++;
        inc[i] -= 0x100;
        r = currow[i];
        if (r >= 0)
        {
            if (r < rows)
                SETSCREENLOCATION(i,r,((random_lfsr() % 94) + 33) | BOLDATTRIB);
            if (--r < rows)
                SETSCREENLOCATION(i,r,(unsigned int)(*((unsigned char far*)&SCREEN_LOC(i,r))) | ATTRIB);    
            r -= curlen[i];
            if (r >= (int)rows)
            {
                currow[i]= random_lfsr() % (2*rows) - rows;
                curlen[i]= random_lfsr() % rows;
                rate[i]= (random_lfsr() % 0xE0)+0x10;
                continue;
            } else if (r >= 0)
                SETSCREENLOCATION(i,r,ATTRIB | 0x0020);
        }
      }
   }
}

