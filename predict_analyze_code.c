/*
stmt -> expr;
        | if (expr) stmt
        | for (optexpr; optexpr; optexpr) stmt
        | other

expr -> expr + term
        | expr - term
        | term

term -> term * factor
        | term / factor
        | factor

factor -> (expr)
        | num

optexpr -> expr | ε

*/

#include "predict_analyze_code.h"

int lookahead;
int numVal;
static char *input = "if (1) for (1; 2; 3) 4 + 5 * 8;8 * 9;";

enum token {
    IF=0, 
    FOR, 
    LPAREN, 
    RPAREN, 
    SEMICOLON, 
    PLUS, 
    MINUS,
    MULTIPLY,
    DIVIDE,
    NUM,
    UNDEF
};


/********** Function Definitions **********/

void stmt() {
    if (lookahead == IF) { // if (expr) stmt
        match(IF);
        match(LPAREN);
        expr();
        match(RPAREN);
        stmt();
    } else if (lookahead == FOR) { // for (optexpr; optexpr; optexpr) stmt
        match(FOR);
        match(LPAREN);
        optexpr();
        match(SEMICOLON);
        optexpr();
        match(SEMICOLON);
        optexpr();
        match(RPAREN);
        stmt();
    } else if (lookahead == NUM || lookahead == LPAREN) {  // expr first set
        expr();
        match(SEMICOLON);
    } else {
        other();
    }
}

void optexpr() {
    if (lookahead != SEMICOLON) {
        expr();
    }
}

void match(int t) {
    if (lookahead == t) {
        printMatch(t);
        lookahead = nextToken();
    } else {
        error();
    }
}

void printMatch(int t) {
    switch (t) {
        case IF: printf("if"); break;
        case FOR: printf("for"); break;
        case LPAREN: printf("("); break;
        case RPAREN: printf(")"); break;
        case SEMICOLON: printf(";"); break;
        case PLUS: printf("+"); break;
        case MINUS: printf("-"); break;
        case MULTIPLY: printf("*"); break;
        case DIVIDE: printf("/"); break;
        case NUM: printf("%d",numVal); break;
    }
    printf(" ");
}

void expr() {
    term();
    while (lookahead == PLUS || lookahead == MINUS) {
        match(lookahead);
        term();
    }
}

void term() {
    factor();
    while (lookahead == MULTIPLY || lookahead == DIVIDE) {
        match(lookahead);
        factor();
    }
}
 
void factor() {
    if (lookahead == LPAREN) {
        match(LPAREN);
        expr();
        match(RPAREN);
    } else if (lookahead == NUM) {
        match(NUM);
    } else {
        error();
    }
}

void other() {
    printf("no match\n");
    exit(1);
}

void error() {
    printf("Syntax error\n");
    exit(1);
}



static int nextToken() {
    static int pos = 0;

    while (input[pos] == ' ' || input[pos] == '\n') {
        pos++;  // Skip whitespace
    }

    if (input[pos] == 'i' && input[pos + 1] == 'f') {
        pos += 2;
        return IF;
    } else if (input[pos] == 'f' && input[pos + 1] == 'o' && input[pos + 2] == 'r') {
        pos += 3;
        return FOR;
    } else if (input[pos] >= '0' && input[pos] <= '9') {
        numVal = input[pos++] - '0';
        return NUM;
    } else {
        switch (input[pos++]) {
            case '(': return LPAREN;
            case ')': return RPAREN;
            case ';': return SEMICOLON;
            case '+': return PLUS;
            case '-': return MINUS;
            case '*': return MULTIPLY;
            case '/': return DIVIDE;
            default: return UNDEF;
        }
    }
}

int main()
{
    lookahead = nextToken();
    while(lookahead != UNDEF) {
        stmt();
    }
    return 0;
}