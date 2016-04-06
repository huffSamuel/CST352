/**********************************************************************
* Author: Samuel Huff
* Filename: prefilter.c
* Date Created: 3/31/2016
* Modifications: 
*
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define GETSIZE 256

/*********************************************************************
*
* Lab/Assignment: Lab 1
*
* Overview: Outputs all characters from <input text> following <delimiter> and including <delimiter>.
*	    Only accepts a single character command line argument. All following characters are ignored.
*
* Input: prefilter <delimiter>
*        <input text>
*
* Output:
*	<delimiter><remaining characters from input text>
*
*	Error 1: Command line arguments not supplied
*	Error 2: Input parse failed
* 	Error 3: Delimiter parse failed
*       Error 4: String output failed
*	Error 5: Unknown fatal error
*********************************************************************/

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    if(fprintf(stderr, "PREFILTER ERROR: Command line argument not supplied\n") > 0)
      return 1;
    else
      exit(5);
  }
  else
  {
    int pid = getpid();
    char delim = *argv[1];
    char inputString[GETSIZE];
    char * charPtr = NULL;

    while(fgets(inputString, GETSIZE, stdin) != NULL)
    {
      charPtr = strchr(inputString, delim);
      if(charPtr != NULL)
      {
        if(printf("%s\r", charPtr) < 0)
        {
          if(fprintf(stderr, "ERROR: String output failed\n") > 0)
            return 4;
          else
            exit(5);
        }
        if(fprintf(stderr, "%d pre %c: %s\r%d rest: %s\r", pid, delim, inputString, pid, charPtr) < 0)
          exit(5);
      }
    }
  }
  return 0;
}
