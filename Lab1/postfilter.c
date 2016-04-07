/*************************************************************
* Author: Samuel Huff
* Filename: postfilter.c
* Date Created: 3/31/2016
* Modifications: 
*
************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define GETSIZE 256

/***********************************************************
*
* Lab/Assignment: Lab 1
*
* Overview: Outputs all character from <input text> following <delimiter> and including <delimiter>.
*           Only accepts a single character from command line argument. All following characters are ignored.
*
* Input: postfilter <delimiter>
*        <input text>
*
* Output:
*	<input text before delimiter><delimiter>
*
*	Error 1: Command line arguments not supplied
*	Error 2: Input parse failed
*	Error 3: Delimiter parse failed
*	Error 4: String output failed
*	Error 5: Unknown fatal error
***********************************************************/

int main(int argc, char * argv[])
{
  if(argc < 2)
  {
    if(fprintf(stderr, "POSTFILTER ERROR: Command line argument not supplied\n") > 0)
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
    int strLength = 0;

    while(fgets(inputString, GETSIZE, stdin) != NULL)
    {
      charPtr = strrchr(inputString, delim);
      strLength = charPtr - inputString + 1;
      if(charPtr != 0 && strLength >= 0)
      {
        if(printf("%.*s\n", strLength, inputString) < 0)
        {
          if(fprintf(stderr, "ERROR: String output failed\n") > 0)
            return 4;
          else
            exit(5);
        }
        if(fprintf(stderr, "%d post %c: %s\r%d rest: %.*s\n", pid, delim, inputString, pid, strLength, inputString) < 0)
          exit(5);
      }
    }
  }
  return 0;
}
