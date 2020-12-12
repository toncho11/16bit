#include <stdio.h>
#include <unistd.h>
#include <termios.h>

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
 
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
//printf("\033[XA"); // Move up X lines;
//printf("\033[XB"); // Move down X lines;
//printf("\033[XC"); // Move right X column;
//printf("\033[XD"); // Move left X column;

int main(void)
{
    int curr_X=0;
	int curr_Y=0;

	clear();
    printf(
        "Enter your number in the box below\n"
        "+-----------------+\n"
        "|                 |\n"
        "+-----------------+\n"
    );
    gotoxy(2, 3);
	
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
					break;
				case 'B':
					// code for arrow down
					printf("\033[1B"); // Move down X lines;
					break;
				case 'C':
					// code for arrow right
					printf("\033[1C"); // Move right X column;
					break;
				case 'D':
					// code for arrow left
					printf("\033[1D"); // Move left X column;
					break;
			}
	   }
	   else 
	   if (curr_ch == '\n')
	   {
		   //printf("%s", "enter");
		   printf("%s", "\r\n");
	   }
	   else
	   {
	       printf("%c", curr_ch);
	   }
    }
	
    return 0;
}

