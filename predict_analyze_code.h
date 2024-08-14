#include <stdio.h>
#include <stdlib.h>



/********** Function Prototypes **********/
static int nextToken();
void stmt();
void expr();
void term();
void factor();
void optexpr();
void match(int t);
void printMatch(int t);
void error();
void other();