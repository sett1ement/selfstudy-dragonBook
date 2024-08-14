#include "ast_simple.h"

char *input_test = "if (1) for (; 2; 3) 4 + 5 * 8; 8 * 9;";

int main() {
    parseWhole();
    return 0;
}