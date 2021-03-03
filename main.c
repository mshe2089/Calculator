#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "functions.h"

//===================================================================================
int main (int argc, char* argv[]) 
{

    //Define initial variables and print opening message
    printf("Cursed Calculator\n");
    int equation_Len;

    //Permanent loop
    while (1)
    {
        //Obtain equation from stdin. 100 character max
        printf(">>> ");
        char* equation_In = (char*)malloc(100 * sizeof(char));
        fgets(equation_In, 100, stdin);
        equation_Len = strlen(equation_In) - 1;

        //Call function to remove whitespace
        compact_equation(equation_In, equation_Len, &equation_Len);

        //Escape loop if q flag detected
        if (equation_Len == 1 && equation_In[0] == 'q')
        {
            break;
        }

        //Print help message if flag h detected
        else if (equation_Len == 1 && equation_In[0] == 'h')
        {
            printf("The following arithmetic operations are valid:\n");
            printf("\n");
            printf("    '^' exponentiation\n");
            printf("    '+' addition\n");
            printf("    '-' subtraction\n");
            printf("    '*' multiplication\n");
            printf("    '/' division\n");
            printf("\n");
            printf("Single-variable algebraic expressions written in terms of 'x' are also valid.\n");
            printf("\n");
            printf("Enter 'h' to display this help message.\n");
            printf("Enter 'q' to quit the program.\n");
        }

        //Call solve equation if it stdin equation is valid and not q or h
        else if (verify_equation(equation_In, equation_Len))
        {
            solve_equation(equation_In);//This will print the result
        }

        //Free equation string
        free(equation_In);
    }

    //Print exit message and terminate when loop is escaped
    printf("Goodbye!\n");
    return 0;
}

