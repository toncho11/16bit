#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
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
int last_line = -1;

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

    old.c_iflag &= ~(IXON); //release Ctrl + S, Ctrl + Q to be used in our program

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
	char* text = (char*)calloc(25 * 80, sizeof(char)); //Currently memory buffer is fixed to 2000!!!
	if( !text ) {fputs("Error on memory allocation!",stderr); exit(1);}
	
	if ( argc != 2 )
    {
        printf( "usage: %s filename", argv[0] );
    }
    else
    {
		//printf("Reading file...");
			
        FILE *file = fopen( argv[1], "r" );

        if ( file == 0 )
        {
            printf( "Error opening file for read\n" );
			exit(1);
        }
        else
        {
			int fd = fileno(file); 
			
			//get file size
			struct stat buf;
			fstat(fd, &buf);
			text_size = buf.st_size;

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
			//printf("\n %d %d",line, line_ends[line]); //for debug
			line++;
		}
	}
	
	//in case there is no '\n' at the end of the file, we need to set the line_ends here:
	//printf("\n test: %d %d %d\n",line, line_ends[line - 1], text_size - 1); //for debug
	if ( line_ends[line - 1] < text_size - 1 ) { line_ends[line] = text_size; line++; }
	
	//printf("\n test: %d \n",line_ends[15]); //for debug
	
	//return 0;
	
	//set last line
	last_line = line - 1;
	
	for (i = line; i< 26; i++) line_ends[i] = -1;
	
	if (line_ends[1] > 0 && text[line_ends[1]-1] == '\r') 
	{
		is_linux_file = 0; //0 means DOS file
		//printf("\n %d\n",is_linux_file); //for debug
		printf("%s","This looks like a DOS file. DOS files are not supported.\nUse: tr -d \'\\015\' < DOS-file > UNIX-file\nExiting ...\n");
		exit(1);
	}
	
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
			
			switch(c)
			{ // the real value that gives the direcion
				case 'A': // code for arrow up - OK

				    //printf("%s","A");
					if (curr_Y>1)
					{
					    int x;
						if (curr_Y == 2) x = curr_X;
						else x = line_ends[curr_Y - 2] + curr_X;
						
						if ( x > line_ends[curr_Y - 1])
						{
							curr_X = line_ends[curr_Y  - 1] - line_ends[curr_Y - 2];
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
					
					if (curr_Y<26 && line_ends[curr_Y+1] != -1)
					{
						//line below current one is shorter: we move down, but we also move right to the end of the next line
						if (line_ends[curr_Y + 1] < line_ends[curr_Y] + curr_X)
						{
							curr_X = line_ends[curr_Y + 1] - line_ends[curr_Y]; //calculates the end of the next line (curr_Y+1)
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
					//we are currently at the new line (at the very right position) and the user clicked to go right: we move down
					if (line_ends[curr_Y - 1] + curr_X == line_ends[curr_Y] && line_ends[curr_Y+1] != -1 )
                    {
						if (curr_Y<25) curr_Y = curr_Y + 1;
						curr_X = 1;
						gotoxy(curr_X,curr_Y);
					}
                    else
					{						
					   if (curr_X<80 && line_ends[curr_Y - 1] + curr_X < line_ends[curr_Y] ) 
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
					//we are at the very left position and the user clicked left: we move up
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
	   if (curr_ch == '\n' || curr_ch == '\r') // Handles "enter", depending on configuration the terminal might be replacing the '\n' with '\r\n'
	   {
		   //printf("%s", "enter");
		   
		   int start_text_pos = line_ends[curr_Y - 1] + curr_X;
		   int size_rest_of_line = line_ends[curr_Y] - curr_X; //??????????????????????????
		   
		   //move right buffer
		   memmove(text + start_text_pos + 1, text + start_text_pos, text_size - start_text_pos);
		   
		   //clear screen from cursor down 
		   printf("\033[J"); 
		   
		   //print new text
		   text[start_text_pos] = '\n';
		   
		   text_size++;
		   text[text_size] = '\0';
		   
		   //print appended line + the trest of the text
		   printf("%s", text + start_text_pos);
		   
		   //new position
		   curr_Y = curr_Y + 1;
		   curr_X = 1;
		   gotoxy(curr_X, curr_Y);
		   
		   //update all: line_ends, last_line, text size
		   last_line++;
		   line_ends[curr_Y] = size_rest_of_line + 1; //?????????????????????????????????????????????????
		   
		   for (int i=curr_Y + 1; i< 26; i++)
			  if (line_ends[i] != -1) line_ends[i]--; 
		   
		   //printf("%s", "\n");
		   //if (curr_Y<25) curr_Y = curr_Y + 1;
		   //curr_X = 1;
  
	   }
	   else if (curr_ch == 127 || curr_ch == '\b') //detect backspace (127 on Ubuntu, '\b' on ELKS) and move text right
	   {
		 if (curr_X != 1) //normal deletion 
		 { 
		   int start_text_pos = line_ends[curr_Y-1] + curr_X;
		   memmove(text + start_text_pos - 1, text + start_text_pos, text_size - start_text_pos);

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
			  if (line_ends[i] != -1) line_ends[i]--; 
		   
		   text_size--;
		   text[text_size]='\0';
		 }
		 else //X=1, we go up and we merge two lines, number of lines is reduced
		 {
		   if (curr_Y != 1)
		   {
			   int start_text_pos = line_ends[curr_Y-1];
			   int size_appended_line = line_ends[curr_Y] - line_ends[curr_Y - 1] - 1;
			   //delete all lines below
			   //for (int i=curr_Y+1; i<=last_line; i++)
			   //{
				//   gotoxy(i,1);
				//  printf("\x1b[2K");
				//   //printf("%c[2K", 27); //\033[K") # Clear to the end of line, printf("\x1b[2K"); clear entire line
			   //}
			   
			   printf("\033[J"); //Clear screen from cursor down 
			   
			   //go up at the end of previous line
			   //printf("%d",line_ends[curr_Y-1]);
			   curr_X = line_ends[curr_Y-1] - line_ends[curr_Y-2];
			   curr_Y = curr_Y - 1;
			   
			   gotoxy(curr_X, curr_Y);
			   //shift up
			   memmove(text + start_text_pos, text + start_text_pos + 1, text_size - start_text_pos);

//#ifdef TEST			   
			   //print merged lines and the line filled with spaces
			   printf("%s", text + start_text_pos);
			   
			   //move cursor to new position
			   gotoxy(curr_X, curr_Y);
			   
			   //correct the text by removing the line filled with spaces (used for deletion of last line)
			   text_size = text_size - 1; //- line_ends[last_line - 1]; //??????????????
			   text[text_size] = '\0';
			   
			   //all line ends that follow must be shifted one character left
			   //curr_Y = line_ends[curr_Y] + size of new text
			   //all others are update + 1????
			   
			   line_ends[curr_Y] = start_text_pos + size_appended_line;
			   for(int i=0; i< last_line - 1; i++) if (line_ends[i] != -1) line_ends[last_line]--;
			   line_ends[last_line] = -1;
			   last_line--;
//#endif
		   }
		 }
	   } //end backspace 
       else 
	   if (curr_ch == 17) //CTRL + Q
	   {
		   //printf("%s", "CTRL + Q");
		   //TODO: must go to the first unused line to put the prompt
		   exit(0);
	   }
	   else 
	   if (curr_ch == 19) //CTRL + S 
	   {
		    //printf("%s", "CTRL + S");
		   
		    FILE *f = fopen("file2.txt", "w");
			if (f == NULL)
			{
				printf("Error opening file for write!\n");
				exit(1);
			}

			fprintf(f, "%s", text);
			
			//start debug 
			fprintf(f, "%s", "|");
			fprintf(f, "\ntext size %d", text_size);
			//fprintf(f, "\nstrlen(text) %d", strlen(text));
			fprintf(f, "\nlast line %d", last_line);
			fprintf(f, "\nline1 %d", line_ends[1]);
			fprintf(f, "\nline2 %d", line_ends[2]);
			fprintf(f, "\nline3 %d", line_ends[3]);
			//end debug
			
			fclose(f);
	   }
	   else //add character and move text right
	   {
		 if (curr_X != 80)
		 { 
	 
	       //problem it does not handle if it is at the end of the file and the end is not enter
		   
           //handles Y=1 correctly because Y0 = -1 and we get: -1 + curr_X = curr_X - 1  
		   int start_text_pos = line_ends[curr_Y-1] + curr_X;
		   memmove(text + start_text_pos + 1, text + start_text_pos, text_size - start_text_pos);

		   for (int i=curr_Y; i< 26; i++)
			   if (line_ends[i]!= -1) line_ends[i]++; 
		   
		   //add character
		   text[start_text_pos] = curr_ch;
		   
		   //already increased by the loop, so the '\0' and set at the correct position
		   text[line_ends[curr_Y]] = '\0'; //to be able to print and then it will be switched to \n 

		   //print at new position
		   printf("%s", text + start_text_pos);
		   
		   text[line_ends[curr_Y]] = '\n';

		   curr_X = curr_X + 1;
		   gotoxy(curr_X, curr_Y);
		   
		   text_size++;
		   text[text_size]='\0';
		 }
	   } //end add character
    } //end while
	
    return 0;
}

