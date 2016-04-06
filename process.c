/**********************************************************************
* Author: Samuel Huff
* Filename: process.c
* Date Created: 3/31/2016
* Modifications: 
*	Samuel Huff: 4/6/2016 - Added paranoid error checking on dup2, exec, and pipe
*
***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define READ 0
#define WRITE 1

/***********************************************************************
*
* Lab/Assignment: Lab 1
*
* Overview: Filters the output of a command <cmd> using the prefilter and postfilter processes.
*	    Process will fork 3 processes: prefilter, postfilter, and the argument command.
*           The output of <cmd> is run through prefilter, then prefilter's output is run through
*             postfilter.
*
* Input: ./process <pre> <post> <cmd> ?<cmdArgs>
*
* Output: Outputs the text from <cmd> ?<cmdArgs> found between <pre> and <post>
*         Example: ./process x y xhelloyworld
*         ::> xhelloy
*
*         Also displays the steps of prefilter and postfilter in stderr.
*
*	  Error 0 - 2: Command line arguments not supplied. Error code is number of arguments supplied.
*	  Error 4: Creation of child process failed
*         Error 5: Creation of pipe failed
*         Error 6: Dup to pipe failed
*         Error 7: Exec of child process failed
*         Error 10: Unknown fatal error
***********************************************************************/

int main(int argc, char *argv[])
{
  if(argc < 4)
  {
    if(fprintf(stderr, "PROCESS ERROR: Command line arguments not supplied\n") > 0)
      return (argc - 1);
    else
      exit(10);
  }
  else
  {
    int count;
    char * argv2[argc - 3];
    int status;
    pid_t pid;
    int P1[2];
    int P2[2];

    for(count = 0; count < argc-2; count++)
      argv2[count] = argv[count+3];

    if(pipe(P1) == -1)
    {
      if(fprintf(stderr, "PROCESS ERROR: Failed to create pipe") > 0)
        return 5;
      else exit(10);
    }

    /***************************Fork Command***********************************/
    pid = fork();
    if(pid == 0)
    {
      if(dup2(P1[WRITE], 1) == -1)
      {
        if(fprintf(stderr, "PROCESS ERROR: Failed to dup to pipe") > 0)
          return 6;
        else exit(10);
      }
      execvpe(argv2[0], argv2);
      if(fprintf(stderr, "PROCESS ERROR: Failed to exec source command") > 0)
        return 7;
      else exit(10);
    }
    else if(pid < 0)
    {
      if(fprintf(stderr, "PROCESS ERROR: Failed to fork process") > 0)
        return 4;
      else
        exit(10);
    }
    /*********END**************************************************************/

    if(pipe(P2) == -1)
    {
      if(fprintf(stderr, "PROCESS ERROR: Failed to create pipe") > 0)
        return 5;
      else exit(10);
    }

    /******************Fork prefilter******************************************/
    pid = fork();
    if(pid == 0)
    {
      close(P1[WRITE]);
      if(dup2(P1[READ], 0) == -1)
      {
        if(fprintf(stderr, "PROCESS ERROR: Failed to dup to pipe\n") > 0)
          return 6;
        else exit(10);
      }
      if(dup2(P2[WRITE], 1) == -1)
      {
        if(fprintf(stderr, "PROCESS ERROR: Failed to dup to pipe\n") > 0)
          return 6;
        else exit(10);
      }
      execlp("./prefilter", "./prefilter", argv[1], NULL);
      if(fprintf(stderr, "PROCESS ERROR: Failed to exec prefilter\n") > 0)
        return 7;
      else exit(10);
    }
    else if (pid < 0)
    {
      if(fprintf(stderr, "PROCESS ERROR: Failed to fork process 'prefilter'") > 0)
        return 4;
      else exit(10);
    }
    /******************END********************************************************/


    /******************Fork postfilter********************************************/
    pid = fork();
    if(pid == 0)
    {
      close(P1[WRITE]);
      close(P1[READ]);
      if(dup2(P2[READ], 0) == -1)
      {
        if(fprintf(stderr, "PROCESS ERROR: Failed to dup to pipe") > 0)
          return 6;
        else exit(10);
      }
      execlp("./postfilter", "./postfilter", argv[2], NULL);
      if(fprintf(stderr, "PROCESS ERROR: Failed to exec postfilter") > 0)
        return 7;
      else exit(10);
    }
    else if(pid < 0)
    {
      if(fprintf(stderr, "PROCESS ERROR: Failed to fork process 'postfilter'") > 0)
        return 4;
      else exit(10); // CHANGE
    }
    /****************END*********************************************************/


    wait(NULL);
    close(P2[0]);
    close(P2[1]);
  }
}
