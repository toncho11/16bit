
This is a "falling letters" screensaver for DOS I wrote for kicks.  It is based on DSCRNSAV 
by David Markovitch and Peter Summers who wrote it over 20 years ago.  It is compiled using
Borland C++ 3.1.

It comes in two flavors:

fallscr.com:  works on CGA,EGA,VGA and uses an auxillary part of video memory for
  the animation as the CRTC of the CGA card supports page flipping.  Works on MGA
  but does not restore the screen properly.
  
mfallscr.com:  works on MGA, CGA,EGA, and VGA, but takes more resident space.  Saves
  the screen contents to a temporary memory buffer to try to approximate a true
  screensaver, but will clobber any changes to the display made while the screensaver is active.

The monochrome version is especially appealing on a real IBM PC green monochrome screen, where
the actual persistence of the phosphors produces the falling letters effect.

The code is broken out so that to modify it to produce other types of text screen savers,
one should just have to modify scrnsav.c.

Have fun with it!

Dan  


/* 

  Falling letters screen saver

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


The original DSCRNSAV text follows:


DSCRNSAV v1.2

DSCRNSAV was originally written by David Markovitch, who I've never met 
or even corresponded with.  My contribution has merely been to improve
an already very useful program.

DSCRNSAV is the smallest screen saver I've ever come across.  It works 
by reprograming the video hardware to move the screen image around, thus 
not requiring any storage space for the screen.  It works in all video 
mode on all the machines I've come across, including CGA, Herc, EGA and 
VGA screens.  I've had one report of a machine on which it wouldn't 
work, and several of programs with seem to deactivate it.

DSCRNSAV could probably be improved further.  I leaves 256 bytes of PSP 
still resident, and doesn't release its environment.  I may look at 
fixing this some other time.  For smallest resident size, load it before 
you start defining environment string (including path and prompt).

Version 1.1 recoded the installation software so that DSCRNSAV could 
find and control the resident copy when it was loaded high, and took
the installation messages out of the resident section of code, reducing 
the TSR size by about 200 bytes.

Version 1.2 allows selectable delays, taking a single command line 
parameter which specifies the delay.  A parameter of zero disables the 
screen saver.

Peter Summers, U5533129@ucsvc.unimelb.edu.au
 
