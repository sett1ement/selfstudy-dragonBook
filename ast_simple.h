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

#include <stdio.h>
#include <stdlib.h>

enum NodeType {
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

struct ASTNode {
    enum NodeType type;
    union
    {
        int numVal; // NUM
        int op; // PLUS, MINUS, MULTIPLY, DIVIDE
    }data;
    struct ASTNode **children;
    int numChildren;
};


struct ASTNode *parseIf();
struct ASTNode *parseFor();
struct ASTNode *stmt();
struct ASTNode *optexpr();
struct ASTNode *expr();
struct ASTNode *term();
struct ASTNode *factor();
struct ASTNode *other();

void parseWhole();
void ASTNode_free(struct ASTNode *node);