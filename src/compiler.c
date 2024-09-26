#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_CODE 1000

typedef struct {
    char type;
    int value;
    char op;
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;

void scan(const char* source) {
    const char* ptr = source;
    while (*ptr) {
        while (isspace(*ptr)) ptr++;
        if (isdigit(*ptr)) {
            tokens[token_count].type = 'P';
            tokens[token_count].value = atoi(ptr);
            while (isdigit(*ptr)) ptr++;
            token_count++;
        } else if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/') {
            tokens[token_count].type = 'O';
            tokens[token_count].op = *ptr;
            token_count++;
            ptr++;
        } else {
            ptr++;
        }
    }
}

void trans(char code[MAX_CODE], char* result) {
    int name_cnt = 0;
    char stack[MAX_TOKENS][10];
    int stack_top = -1;

    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == 'P') {
            sprintf(code + strlen(code), "int t%d = %d;\n", name_cnt, tokens[i].value);
            sprintf(stack[++stack_top], "t%d", name_cnt);
            name_cnt++;
        } else if (tokens[i].type == 'O') {
            char a[10], b[10];
            strcpy(a, stack[stack_top--]);
            strcpy(b, stack[stack_top--]);
            sprintf(code + strlen(code), "int t%d = %s %c %s;\n", name_cnt, b, tokens[i].op, a);
            sprintf(stack[++stack_top], "t%d", name_cnt);
            name_cnt++;
        }
    }
    strcpy(result, stack[stack_top]);
}

void rpn_to_c(const char* source) {
    char code[MAX_CODE] = "";
    char result[10];

    scan(source);
    trans(code, result);

    printf("#include <stdio.h>\n");
    printf("int main() {\n");
    printf("%s", code);
    printf("printf(\"%%d\\n\", %s);\n", result);
    printf("return 0;\n");
    printf("}\n");
}

int main() {
    rpn_to_c("2 2 +");
    return 0;
}
