#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#define clear() printf("\033[H\033[J")
#define CLEARSEQ "\x1b[H\x1b[2J"
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
//printf("\033[XA"); // Move up X lines;
//printf("\033[XB"); // Move down X lines;
//printf("\033[XC"); // Move right X column;
//printf("\033[XD"); // Move left X column;


#define SHOW_POS

int curr_X=1; //max 80
int curr_Y=1; //max 25

void clear(void)
{
	 write(STDOUT_FILENO, CLEARSEQ, 7);
}

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    //printf("%c\n", buf);
    return buf;
 }
 
 void showpos(void)
 {
	 gotoxy(1, 25);
	 printf("[%d | %d]  ",curr_X,curr_Y);
	 gotoxy(curr_X, curr_Y);
 }
 
int main(int argc, char *argv[])
{
	char* text = (char*)calloc(25 * 80, sizeof(char));
	
	clear();
    //sprintf(text, "%s","Enter your number in the box below\n+-----------------+\n|                 |\n+-----------------+\n");
	if ( argc != 2 )
    {
        printf( "usage: %s filename", argv[0] );
    }
    else
    {
        // We assume argv[1] is a filename to open
        FILE *file = fopen( argv[1], "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 )
        {
            printf( "Could not open file\n" );
			exit(1);
        }
        else
        {
			int fd = fileno(file); 
			struct stat buf;
			fstat(fd, &buf);
			int lSize = buf.st_size;

            // fseek( file , 0L , SEEK_END);
            // int lSize = ftell( file );
            // rewind( file ); //fseek(fp, 0L, SEEK_SET);

/* allocate memory for entire content */
//buffer = calloc( 1, lSize+1 );
//if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

            /* copy the file into the buffer */
			fread( text , lSize, 1 , file);
            //if( lSize!=fread( text , lSize, 1 , file) ) //2000!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//{
             //    fclose(file);free(text);fputs("entire read fails\n",stderr);exit(1);
			//}
            //fclose( file );
        }
    }
	
	printf("%s",text);
	
	curr_X = 1;
	curr_Y = 1;
	
    gotoxy(curr_X, curr_Y);
	
    while(1)
	{
	   char curr_ch = getch();
	   
	   if (curr_ch == '\033')
	   {    // if the first value is esc
            getch(); // skip the [
			switch(getch())
			{ // the real value
				case 'A':
					// code for arrow up
					printf("\033[1A"); // Move up X lines;
					if (curr_Y>1) curr_Y = curr_Y - 1;
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'B':
					// code for arrow down
					printf("\033[1B"); // Move down X lines;
					if (curr_Y<25) curr_Y = curr_Y + 1;
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'C':
					// code for arrow right
					printf("\033[1C"); // Move right X column;
					if (curr_X<80) curr_X = curr_X + 1;
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'D':
					// code for arrow left
					printf("\033[1D"); // Move left X column;
					if (curr_X>1) curr_X = curr_X - 1;
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
			}
	   }
	   else 
	   if (curr_ch == '\n' || curr_ch == '\r') //ubuntu works with \n and ELKS works with \r
	   {
		   //printf("%s", "enter");
		   printf("%s", "\r\n");
		   if (curr_Y<25) curr_Y = curr_Y + 1;
		   curr_X = 1;
		   
		   gotoxy(1, 25);
		   printf("[%d | %d]  ",curr_X,curr_Y);
		   gotoxy(curr_X, curr_Y);
	   } 
	   else
	   {
	       printf("%c", curr_ch);
	   }
    }
	
    return 0;
}

