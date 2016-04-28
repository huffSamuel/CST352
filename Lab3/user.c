/****************************************************************
* Author: Samuel Huff
* Filename: user.c
* Date Created: April 27th, 2016
* Modifications: 
****************************************************************/
#include "userLib.h"

/********************************************************************** 
* Purpose: Tests the functionality of "userland" code on the STACKL system.
*
* Precondition: 
*   None.
*
* Postcondition: 
*   User library code is run. See userLib.h for detailed information on these
*   functions.
*
************************************************************************/
int main()
{
    char buff[30];
    int temp;
    // Testing printing functions
    prints("Testing prints\n");
    prints("Testing printi(5)\n");
    printi(5);
    prints("\n");

    //Testing get and print
    prints("Enter a string: ");
    gets(buff);
    prints("You entered: ");
    prints(buff);  
    prints("Enter a number: ");
    gets(buff);
    temp = geti();
    prints("You entered: ");
    printi(temp);
    prints("\n");

    return 0;
}