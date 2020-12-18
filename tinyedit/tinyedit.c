#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>

//#define clear() printf("\033[H\033[J")
#define CLEARSEQ "\x1b[H\x1b[2J"
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
//printf("\033[XA"); // Move up X lines;
//printf("\033[XB"); // Move down X lines;
//printf("\033[XC"); // Move right X column;
//printf("\033[XD"); // Move left X column;


#define SHOW_POS

int curr_X = 1; //max 80
int curr_Y = 1; //max 25
int line_ends[26];
int text_size = 0;
int is_linux_file = 1; //only '\n'
int buff_X = -1;

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

    old.c_iflag &= ~(IXON); //disable Ctrl + S, Ctrl + Q

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
	
    //printf("Reading file...");
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
			text_size = buf.st_size;

            // fseek( file , 0L , SEEK_END);
            // int lSize = ftell( file );
            // rewind( file ); //fseek(fp, 0L, SEEK_SET);

			/* allocate memory for entire content */
			//buffer = calloc( 1, lSize+1 );
			//if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

            /* copy the file into the buffer */
			fread( text , text_size, 1 , file);
            text[text_size] = '\0';
            //if( lSize!=fread( text , lSize, 1 , file) ) //2000!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//{
             //    fclose(file);free(text);fputs("entire read fails\n",stderr);exit(1);
			//}
            fclose( file );
        }
    }
	
    clear();
		
	int i=0;
	int line = 1;
	line_ends[0] = -1;
	for (int i=0; i < text_size; i++)
	{
		if (text[i] == '\n')
		{
			line_ends[line] = i;
			//printf("\n %d",line_ends[line]);
			line++;
			
		}
	}
	for (i = line; i< 26; i++) line_ends[i] = -1;
	
	if (line_ends[0] == '\r') is_linux_file = 0; //0 means DOS file
	//printf("\n %d\n",is_linux_file);
	
	printf("%s",text);
	
	//return 0;
	curr_X = 1;
	curr_Y = 1;
	
    gotoxy(curr_X, curr_Y);
	
    while(1)
	{
	   char curr_ch = getch();
	   
	   if (curr_ch == '\033')
	   {    // if the first value is esc
            getch(); // skip the [
			char c = getch();
			
			//int x_now = line_ends[curr_Y-1] + curr_X;
			
			switch(c)
			{ // the real value
				case 'A': // code for arrow up - OK

				    //printf("%s","A");
					if (curr_Y>1)
					{
					    int x;
						if (curr_Y == 2) x = curr_X;
						else x = line_ends[curr_Y - 2] + curr_X;
						
						if ( x > line_ends[curr_Y-1])
						{
							curr_X = line_ends[curr_Y  - 1] - line_ends[curr_Y  - 2];
						}
						
						curr_Y = curr_Y - 1;
					    gotoxy(curr_X,curr_Y);
					}
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'B': // code for arrow down - OK
					
					//printf("%s","B");
					//printf("\033[1B"); // Move down X lines;
					
					if (curr_Y<26 && line_ends[curr_Y+1] != -1)
					{
						if (line_ends[curr_Y + 1] < line_ends[curr_Y] + curr_X) //problem
						{
							//curr_X = line_ends[curr_Y + 1] - line_ends[curr_Y] ;
							curr_X = line_ends[curr_Y + 1] - line_ends[curr_Y] ;
						}
						
						curr_Y = curr_Y + 1;
						gotoxy(curr_X,curr_Y);
					}
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'C': // code for arrow right
					
					//printf("%s","C");
					if (line_ends[curr_Y - 1] + curr_X == line_ends[curr_Y] && line_ends[curr_Y+1] != -1 ) //we are currently at the new line and the user clicked to go right
                    {
						if (curr_Y<25) curr_Y = curr_Y + 1;
						curr_X = 1;
						gotoxy(curr_X,curr_Y);
					}
                    else
					{						
					   if (curr_X<80 && line_ends[curr_Y -1] + curr_X < line_ends[curr_Y] ) 
					   {
						   curr_X = curr_X + 1;
						   gotoxy(curr_X, curr_Y);
					   }
					   
					}					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				case 'D': // code for arrow left - OK
					
					//printf("%s","D");
					if (curr_X == 1 && curr_Y > 1)
                    {
						curr_X = line_ends[curr_Y-1] - line_ends[curr_Y-2];
						curr_Y = curr_Y - 1;
						gotoxy(curr_X,curr_Y);
					}
                    else
					{	
					  printf("\033[1D"); // Move left X column;
					  if (curr_X>1) curr_X = curr_X - 1;
					}
					
					#ifdef SHOW_POS
					showpos();
					#endif
					
					break;
				default: 
				    printf("%s %c","not handled:",c); //for debug purposes
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
           else if (curr_ch == 127 || curr_ch == '\b') //detect backspace (127 on Ubuntu, '\b' on ELKS) and move text right
           {
             if (curr_X != 1)
             { 
		       int start_text_pos = line_ends[curr_Y-1] + curr_X;
               memcpy(text + start_text_pos - 1, text + start_text_pos, text_size - start_text_pos);

               //add extra space to delete last character
               text[line_ends[curr_Y] - 1] = ' ';
			   char c = text[line_ends[curr_Y]];
               text[line_ends[curr_Y]] = '\0';

               //print at new position
			   gotoxy(curr_X - 1, curr_Y);
               printf("%s", text + start_text_pos - 1);
               
               //remove space
               text[line_ends[curr_Y] -1 ] = '\n'; //put the new end of line
			   text[line_ends[curr_Y]] = c;
               
			   curr_X = curr_X - 1;
               gotoxy(curr_X, curr_Y);
               
               for (int i=curr_Y; i< 26; i++)
				  line_ends[i]--; 
			   
			   text_size--;
			   text[text_size]='\0';
             }
			 else
			 {
			   int start_text_pos = line_ends[curr_Y-1];
               memcpy(text + start_text_pos, text + start_text_pos, text_size - start_text_pos);
			 }
           } 
	   else //print character and move all characters right
	   if (curr_ch == 17) //CTRL + Q
	   {
		   printf("%s", "CTRL + Q");
	   }
	   else 
	   if (curr_ch == 19) //CTRL + S 
	   {
		    //printf("%s", "CTRL + S");
		   
		    FILE *f = fopen("file2.txt", "w");
			if (f == NULL)
			{
				printf("Error opening file!\n");
				exit(1);
			}

			/* print some text */
			fprintf(f, "%s", text);
			
			//debug 
			fprintf(f, "\ntext size %d", text_size);
			fprintf(f, "\nline1 %d", line_ends[1]);
			fprintf(f, "\nline2 %d", line_ends[2]);
			fprintf(f, "\nline3 %d", line_ends[12]);
			

			fclose(f);
	   }
	   else //add character and move text right
           {
             if (curr_X != 80)
             {  
  		       int start_text_pos = line_ends[curr_Y-1] + curr_X;
			   memcpy(text + start_text_pos + 1, text + start_text_pos, text_size - start_text_pos);

			   for (int i=curr_Y; i< 26; i++)
				   line_ends[i]++; 
			   
               //add character
               text[start_text_pos] = curr_ch;
               text[line_ends[curr_Y]] = '\0'; //to be able to print and then it will be switched to \n 

               //print at new position
               printf("%s", text + start_text_pos);

			   curr_X = curr_X + 1;
               gotoxy(curr_X, curr_Y);
               
			   text[line_ends[curr_Y]] = '\n';
			   
			   text_size++;
			   text[text_size]='\0';
             }
           } 
    }
	
    return 0;
}

