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
#include "ast_simple.h"

#define stmtNUM 4


int lookahead;
int numVal;
static char *input = "if (1) for (2; ; 3) 4 + 5 * 8; 8 * 9;";


/********** Static Function Definitions **********/
static int nextToken();
void match(int t);
void error();
static struct ASTNode *newNode(enum NodeType type);
static void printAST(struct ASTNode *node, int depth);

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

void match(int t) {
    if (lookahead == t) {
        lookahead = nextToken();
    } else {
        error();
    }
}

void error() {
    printf("Syntax error\n");
    exit(1);
}


static struct ASTNode *newNode(enum NodeType type) {
    struct ASTNode *node = (struct ASTNode *)malloc(sizeof(struct ASTNode));
    node->type = type;
    node->children = NULL;
    node->numChildren = 0;
    return node;
}


static void printAST(struct ASTNode *node, int depth) {
    printf("(%d)", depth);
    // Print node data
    switch (node->type) {
        case NUM:
            printf("%d", node->data.numVal);
            break;
        case PLUS:
            printf("+");
            break;
        case MINUS:
            printf("-");
            break;
        case MULTIPLY:
            printf("*");
            break;
        case DIVIDE:
            printf("/");
            break;
        case IF:
            printf("if");
            break;
        case FOR:
            printf("for");
            break;
        case UNDEF:
            printf("undef");
            break;
    }
    printf(" ");

    // Print children
    for (int i = 0; i < node->numChildren; i++) {
        printf("(parent:");
        switch (node->type) {
        case NUM:
            printf("%d", node->data.numVal);
            break;
        case PLUS:
            printf("+");
            break;
        case MINUS:
            printf("-");
            break;
        case MULTIPLY:
            printf("*");
            break;
        case DIVIDE:
            printf("/");
            break;
        case IF:
            printf("if");
            break;
        case FOR:
            printf("for");
            break;
        case UNDEF:
            printf("undef");
            break;
        }
        printf(")");
        
        if (node->children[i] != NULL) {
            printAST(node->children[i], depth + 1);
        } else {
            printf("(%d)NULL ", depth + 1);
        }
    }

    if (depth == 0) {
        printf("\n");
    }
}


/********** Function Definitions **********/
struct ASTNode *parseIf()
{
    struct ASTNode *node = newNode(IF);
    match(IF);
    match(LPAREN);
    node->children = (struct ASTNode **)malloc(sizeof(struct ASTNode *) * 2);
    node->numChildren = 2;
    node->children[0] = expr();
    match(RPAREN);
    node->children[1] = stmt();
    return node;
}

struct ASTNode *parseFor()
{
    struct ASTNode *node = newNode(FOR);
    match(FOR);
    match(LPAREN);
    node->children = (struct ASTNode **)malloc(sizeof(struct ASTNode *) * 4);
    node->numChildren = 4;
    node->children[0] = optexpr();
    match(SEMICOLON);
    node->children[1] = optexpr();
    match(SEMICOLON);
    node->children[2] = optexpr();
    match(RPAREN);
    node->children[3] = stmt();
    return node;
}


struct ASTNode *stmt() {
    struct ASTNode *node;
    if (lookahead == IF) { // if (expr) stmt
        node = parseIf();
    } else if (lookahead == FOR) { // for (optexpr; optexpr; optexpr) stmt
        node = parseFor();
    } else if (lookahead == NUM || lookahead == LPAREN) {  // expr first set
        node = expr();
        match(SEMICOLON);
    } else {
        node = other();
    }
    return node;
}

struct ASTNode *other() {
    struct ASTNode *node = newNode(UNDEF);
    // haven't implemented other productions yet
    printf("UNDEF NOW\n");
    exit(1);

    return node;
}

struct ASTNode *optexpr() {
    if (lookahead != SEMICOLON) {
        return expr();
    }
    return NULL;
}

struct ASTNode *expr() {
    struct ASTNode *node = term();
    while (lookahead == PLUS || lookahead == MINUS) {
        struct ASTNode *newNode = (struct ASTNode *)malloc(sizeof(struct ASTNode));
        newNode->type = lookahead;
        newNode->data.op = lookahead;
        newNode->children = (struct ASTNode **)malloc(sizeof(struct ASTNode *) * 2);
        newNode->numChildren = 2;
        newNode->children[0] = node;
        match(lookahead);
        newNode->children[1] = term();
        node = newNode;
    }
    return node;
}

struct ASTNode *term() {
    struct ASTNode *node = factor();
    while (lookahead == MULTIPLY || lookahead == DIVIDE) {
        struct ASTNode *newNode = (struct ASTNode *)malloc(sizeof(struct ASTNode));
        newNode->type = lookahead;
        newNode->data.op = lookahead;
        newNode->children = (struct ASTNode **)malloc(sizeof(struct ASTNode *) * 2);
        newNode->numChildren = 2;
        newNode->children[0] = node;
        match(lookahead);
        newNode->children[1] = factor();
        node = newNode;
    }
    return node;
}

struct ASTNode *factor() {
    struct ASTNode *node;
    if (lookahead == LPAREN) {
        match(LPAREN);
        node = expr();
        match(RPAREN);
    } else if (lookahead == NUM) {
        node = newNode(NUM);
        node->data.numVal = numVal;
        match(NUM);
    } else {
        error();
    }
    return node;
}


void parseWhole() {
    lookahead = nextToken();
    int count = 0;

    struct ASTNode **root = (struct ASTNode **)malloc(sizeof(struct ASTNode *) * stmtNUM);
    while(lookahead != UNDEF) {
        // root[count++] = stmt();
        // printAST(root[count - 1], 0);
        // ASTNode_free(root[count - 1]);

        root[0] = stmt();
        printAST(root[0], 0);
        ASTNode_free(root[0]);
    }

}


void ASTNode_free(struct ASTNode *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->numChildren; i++) {
        ASTNode_free(node->children[i]);
    }
    free(node->children);
    free(node);
}
