#include <avr/io.h> 
#define F_CPU 16000000
#include <util/delay.h> 
#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include "tinyexpr.h"
// Processer Requirements
#define MAX_LENGTH 100
#define NUM_STRINGS 10
#define FUNCTIONS_STRINGS 7
#define OPERATORS_STRINGS 3
#define INVERSE_STRINGS 3

char* num[NUM_STRINGS] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
char* function[FUNCTIONS_STRINGS] = {"sin(", "cos(", "tan(", "log(", "ln(", "pow(", "exp("};
char* operators[OPERATORS_STRINGS] = {"+", "-", "*", "/", "(", ")", ","};
char* inverse[INVERSE_STRINGS] = {"atan(", "asin(", "acos("};

char** str = NULL; // Main Expression
int length = 0;
int cursor = 0;
int currentStringIndex = 0; 
char** currentStrings = num; 
int numCurrentStrings = NUM_STRINGS; 

char** lists[] = {num, function, operators, inverse};
int listLengths[] = {NUM_STRINGS, FUNCTIONS_STRINGS, OPERATORS_STRINGS, INVERSE_STRINGS};
int currentListIndex = 0;
int previousListIndex = 0;

// TYPEDEFS
typedef uint8_t byte; // changed the name

#define CLEARDISPLAY 0x01
extern void delay(void);
extern void SendByte(byte data);
extern void PulseEnableLine(void);
extern void LCD_Cmd(byte data);
extern void LCD_Char(byte data);
extern void LCD_Init(void);
int isalphaa(char ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        return 1;
    } else {
        return 0;
    }
}

int isdigitt(char ch){
    if((ch >= '0' && ch <= '9')) {
        return 1;
    } else {
        return 0;
    }
}
 

void LCD_Clear() // clear the LCD display
{
 LCD_Cmd(CLEARDISPLAY);
 _delay_ms(3); // wait for LCD to process command
}

void LCD_Message(const char *text) // display string on LCD
{
 while (*text) LCD_Char(*text++); // send char & update char pointer
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
 char st[8] = ""; // save enough space for result
 itoa(data,st,10); // 
 LCD_Message(st); // display in on LCD
}
void processInput(int input) {
    switch (input) {
        case 1: 
            currentStringIndex = (currentStringIndex + 1) % numCurrentStrings;
            if (cursor >= length) {
                if (length < MAX_LENGTH) {
                    str[length] = currentStrings[currentStringIndex];
                    length++;
                }
            } else {
                str[cursor] = currentStrings[currentStringIndex];
            }
            break;
        case 2: 
            currentStringIndex = (currentStringIndex - 1 + numCurrentStrings) % numCurrentStrings;
            if (cursor >= length) {
                break;
            }
            str[cursor] = currentStrings[currentStringIndex];
            break;
        case 3: 
            if (cursor < length - 1) {
                cursor++;
            } else if (length < MAX_LENGTH) {
                cursor++;
                str[length] = currentStrings[0]; 
                length++;
            }
            currentStringIndex = 0; 
            break;
        case 4: 
            if (cursor > 0) {
                cursor--;
            }
            break;
        case 5:
            if (cursor > 0) {
                for (int j = cursor - 1; j < length - 1; j++) {
                    str[j] = str[j + 1];
                }
                length--;
                cursor--;
            }
            break;
        case 6: 
            previousListIndex = currentListIndex;
            currentListIndex = (currentListIndex + 1) % 4;
            currentStrings = lists[currentListIndex];
            numCurrentStrings = listLengths[currentListIndex];
            currentStringIndex = 0; 
            break;
        case 7:
            currentListIndex = previousListIndex;
            currentStrings = lists[currentListIndex];
            numCurrentStrings = listLengths[currentListIndex];
            currentStringIndex = 0; 
            break;
        case 8: 
            currentListIndex = 3;
            currentStrings = lists[currentListIndex];
            numCurrentStrings = listLengths[currentListIndex];
            currentStringIndex = 0; 
            break;
        default:
            //printf("Invalid input\n");
            break;
    }
}

void loop(void){
  LCD_Clear();
  LCD_Cmd(0x80); // Force cursor to line 1
  _delay_ms(100);     // set animation speed

}

int main(void)
{
 TCCR0B = 0x05;

// use PortB for LCD interface
 DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 

 LCD_Init(); // initialize LCD controller
const char *expression = "pow(2,0.5)";
    //printf("Evaluating:\n\t%s\n", expression);

    int err;
    te_expr *n = te_compile(expression, 0, 1, &err);

    if (n) {
        const double r = te_eval(n);
        //printf("Result:\n\t%f\n", r);
        te_free(n);
    } else {
        /* Show the user where the error is at. */
        //printf("\t%*s^\nError near here", err-1, "");
    }


    return 0;
 while(1)
 {
  loop();
}
}
#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif


typedef double (*te_fun2)(double, double);

enum {
    TOK_NULL = TE_CLOSURE7+1, TOK_ERROR, TOK_END, TOK_SEP,
    TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, TOK_INFIX
};


enum {TE_CONSTANT = 1};


typedef struct state {
    const char *start;
    const char *next;
    int type;
    union {double value; const double *bound; const void *function;};
    void *context;

    const te_variable *lookup;
    int lookup_len;
} state;


#define TYPE_MASK(TYPE) ((TYPE)&0x0000001F)

#define IS_PURE(TYPE) (((TYPE) & TE_FLAG_PURE) != 0)
#define IS_FUNCTION(TYPE) (((TYPE) & TE_FUNCTION0) != 0)
#define IS_CLOSURE(TYPE) (((TYPE) & TE_CLOSURE0) != 0)
#define ARITY(TYPE) ( ((TYPE) & (TE_FUNCTION0 | TE_CLOSURE0)) ? ((TYPE) & 0x00000007) : 0 )
#define NEW_EXPR(type, ...) new_expr((type), (const te_expr*[]){__VA_ARGS__})
#define CHECK_NULL(ptr, ...) if ((ptr) == NULL) { __VA_ARGS__; return NULL; }

static te_expr *new_expr(const int type, const te_expr *parameters[]) {
    const int arity = ARITY(type);
    const int psize = sizeof(void*) * arity;
    const int size = (sizeof(te_expr) - sizeof(void*)) + psize + (IS_CLOSURE(type) ? sizeof(void*) : 0);
    te_expr *ret = malloc(size);
    CHECK_NULL(ret);

    memset(ret, 0, size);
    if (arity && parameters) {
        memcpy(ret->parameters, parameters, psize);
    }
    ret->type = type;
    ret->bound = 0;
    return ret;
}


void te_free_parameters(te_expr *n) {
    if (!n) return;
    switch (TYPE_MASK(n->type)) {
        case TE_FUNCTION7: case TE_CLOSURE7: te_free(n->parameters[6]);     /* Falls through. */
        case TE_FUNCTION6: case TE_CLOSURE6: te_free(n->parameters[5]);     /* Falls through. */
        case TE_FUNCTION5: case TE_CLOSURE5: te_free(n->parameters[4]);     /* Falls through. */
        case TE_FUNCTION4: case TE_CLOSURE4: te_free(n->parameters[3]);     /* Falls through. */
        case TE_FUNCTION3: case TE_CLOSURE3: te_free(n->parameters[2]);     /* Falls through. */
        case TE_FUNCTION2: case TE_CLOSURE2: te_free(n->parameters[1]);     /* Falls through. */
        case TE_FUNCTION1: case TE_CLOSURE1: te_free(n->parameters[0]);
    }
}


void te_free(te_expr *n) {
    if (!n) return;
    te_free_parameters(n);
    free(n);
}


static double pi(void) {return 3.141;}
static double e(void) {return 2.718;}

#ifdef _MSC_VER
#pragma function (ceil)
#pragma function (floor)
#endif

static const te_variable functions[] = {
    /* must be in alphabetical order */
    {"cos", mcos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"e", e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
    {"exp", mexp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"ln", mln,       TE_FUNCTION1 | TE_FLAG_PURE, 0},
#ifdef TE_NAT_LOG
    {"log", mlog,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
#else
    {"log", mlog,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
#endif
    {"log10", mlog,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"pi", pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
    {"pow", mpow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
    {"sin", msin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"tan", mtan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {0, 0, 0, 0}
};

static const te_variable *find_builtin(const char *name, int len) {
    int imin = 0;
    int imax = sizeof(functions) / sizeof(te_variable) - 2;

    /*Binary search.*/
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        int c = strncmp(name, functions[i].name, len);
        if (!c) c = '\0' - functions[i].name[len];
        if (c == 0) {
            return functions + i;
        } else if (c > 0) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}

static const te_variable *find_lookup(const state *s, const char *name, int len) {
    int iters;
    const te_variable *var;
    if (!s->lookup) return 0;

    for (var = s->lookup, iters = s->lookup_len; iters; ++var, --iters) {
        if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') {
            return var;
        }
    }
    return 0;
}



static double add(double a, double b) {return a + b;}
static double sub(double a, double b) {return a - b;}
static double mul(double a, double b) {return a * b;}
static double divide(double a, double b) {return a / b;}
static double negate(double a) {return -a;}
static double comma(double a, double b) {(void)a; return b;}


void next_token(state *s) {
    s->type = TOK_NULL;

    do {

        if (!*s->next){
            s->type = TOK_END;
            return;
        }

        /* Try reading a number. */
        if ((s->next[0] >= '0' && s->next[0] <= '9') || s->next[0] == '.') {
            s->value = strtod(s->next, (char**)&s->next);
            s->type = TOK_NUMBER;
        } else {
            /* Look for a variable or builtin function call. */
            if (isalphaa(s->next[0])) {
                const char *start;
                start = s->next;
                while (isalphaa(s->next[0]) || isdigitt(s->next[0]) || (s->next[0] == '_')) s->next++;
                
                const te_variable *var = find_lookup(s, start, s->next - start);
                if (!var) var = find_builtin(start, s->next - start);

                if (!var) {
                    s->type = TOK_ERROR;
                } else {
                    switch(TYPE_MASK(var->type))
                    {
                        case TE_VARIABLE:
                            s->type = TOK_VARIABLE;
                            s->bound = var->address;
                            break;

                        case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:         /* Falls through. */
                        case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:         /* Falls through. */
                            s->context = var->context;                                                  /* Falls through. */

                        case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:     /* Falls through. */
                        case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:     /* Falls through. */
                            s->type = var->type;
                            s->function = var->address;
                            break;
                    }
                }

            } else {
                /* Look for an operator or special character. */
                switch (s->next++[0]) {
                    case '+': s->type = TOK_INFIX; s->function = add; break;
                    case '-': s->type = TOK_INFIX; s->function = sub; break;
                    case '*': s->type = TOK_INFIX; s->function = mul; break;
                    case '/': s->type = TOK_INFIX; s->function = divide; break;
                    //case '^': s->type = TOK_INFIX; s->function = pow; break;
                    //case '%': s->type = TOK_INFIX; s->function = fmod; break;
                    case '(': s->type = TOK_OPEN; break;
                    case ')': s->type = TOK_CLOSE; break;
                    case ',': s->type = TOK_SEP; break;
                    case ' ': case '\t': case '\n': case '\r': break;
                    default: s->type = TOK_ERROR; break;
                }
            }
        }
    } while (s->type == TOK_NULL);
}


static te_expr *list(state *s);
static te_expr *expr(state *s);
static te_expr *power(state *s);

static te_expr *base(state *s) {
    /* <base>      =    <constant> | <variable> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
    te_expr *ret;
    int arity;

    switch (TYPE_MASK(s->type)) {
        case TOK_NUMBER:
            ret = new_expr(TE_CONSTANT, 0);
            CHECK_NULL(ret);

            ret->value = s->value;
            next_token(s);
            break;

        case TOK_VARIABLE:
            ret = new_expr(TE_VARIABLE, 0);
            CHECK_NULL(ret);

            ret->bound = s->bound;
            next_token(s);
            break;

        case TE_FUNCTION0:
        case TE_CLOSURE0:
            ret = new_expr(s->type, 0);
            CHECK_NULL(ret);

            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[0] = s->context;
            next_token(s);
            if (s->type == TOK_OPEN) {
                next_token(s);
                if (s->type != TOK_CLOSE) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
            }
            break;

        case TE_FUNCTION1:
        case TE_CLOSURE1:
            ret = new_expr(s->type, 0);
            CHECK_NULL(ret);

            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[1] = s->context;
            next_token(s);
            ret->parameters[0] = power(s);
            CHECK_NULL(ret->parameters[0], te_free(ret));
            break;

        case TE_FUNCTION2: case TE_FUNCTION3: case TE_FUNCTION4:
        case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
        case TE_CLOSURE2: case TE_CLOSURE3: case TE_CLOSURE4:
        case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
            arity = ARITY(s->type);

            ret = new_expr(s->type, 0);
            CHECK_NULL(ret);

            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[arity] = s->context;
            next_token(s);

            if (s->type != TOK_OPEN) {
                s->type = TOK_ERROR;
            } else {
                int i;
                for(i = 0; i < arity; i++) {
                    next_token(s);
                    ret->parameters[i] = expr(s);
                    CHECK_NULL(ret->parameters[i], te_free(ret));

                    if(s->type != TOK_SEP) {
                        break;
                    }
                }
                if(s->type != TOK_CLOSE || i != arity - 1) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
            }

            break;

        case TOK_OPEN:
            next_token(s);
            ret = list(s);
            CHECK_NULL(ret);

            if (s->type != TOK_CLOSE) {
                s->type = TOK_ERROR;
            } else {
                next_token(s);
            }
            break;

        default:
            ret = new_expr(0, 0);
            CHECK_NULL(ret);

            s->type = TOK_ERROR;
            ret->value = NAN;
            break;
    }

    return ret;
}


static te_expr *power(state *s) {
    /* <power>     =    {("-" | "+")} <base> */
    int sign = 1;
    while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
        if (s->function == sub) sign = -sign;
        next_token(s);
    }

    te_expr *ret;

    if (sign == 1) {
        ret = base(s);
    } else {
        te_expr *b = base(s);
        CHECK_NULL(b);

        ret = NEW_EXPR(TE_FUNCTION1 | TE_FLAG_PURE, b);
        CHECK_NULL(ret, te_free(b));

        ret->function = negate;
    }

    return ret;
}

#ifdef TE_POW_FROM_RIGHT
static te_expr *factor(state *s) {
    /* <factor>    =    <power> {"^" <power>} */
    te_expr *ret = power(s);
    CHECK_NULL(ret);

    int neg = 0;

    if (ret->type == (TE_FUNCTION1 | TE_FLAG_PURE) && ret->function == negate) {
        te_expr *se = ret->parameters[0];
        free(ret);
        ret = se;
        neg = 1;
    }

    te_expr *insertion = 0;

    while (s->type == TOK_INFIX && (s->function == mpow)) {
        te_fun2 t = s->function;
        next_token(s);

        if (insertion) {
            /* Make exponentiation go right-to-left. */
            te_expr *p = power(s);
            CHECK_NULL(p, te_free(ret));

            te_expr *insert = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, insertion->parameters[1], p);
            CHECK_NULL(insert, te_free(p), te_free(ret));

            insert->function = t;
            insertion->parameters[1] = insert;
            insertion = insert;
        } else {
            te_expr *p = power(s);
            CHECK_NULL(p, te_free(ret));

            te_expr *prev = ret;
            ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, p);
            CHECK_NULL(ret, te_free(p), te_free(prev));

            ret->function = t;
            insertion = ret;
        }
    }

    if (neg) {
        te_expr *prev = ret;
        ret = NEW_EXPR(TE_FUNCTION1 | TE_FLAG_PURE, ret);
        CHECK_NULL(ret, te_free(prev));

        ret->function = negate;
    }

    return ret;
}
#else
static te_expr *factor(state *s) {
    /* <factor>    =    <power> {"^" <power>} */
    te_expr *ret = power(s);
    CHECK_NULL(ret);

    while (s->type == TOK_INFIX && (s->function == mpow)) {
        te_fun2 t = s->function;
        next_token(s);
        te_expr *p = power(s);
        CHECK_NULL(p, te_free(ret));

        te_expr *prev = ret;
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, p);
        CHECK_NULL(ret, te_free(p), te_free(prev));

        ret->function = t;
    }

    return ret;
}
#endif



static te_expr *term(state *s) {
    /* <term>      =    <factor> {("*" | "/" | "%") <factor>} */
    te_expr *ret = factor(s);
    CHECK_NULL(ret);

    while (s->type == TOK_INFIX && (s->function == mul || s->function == divide)) {
        te_fun2 t = s->function;
        next_token(s);
        te_expr *f = factor(s);
        CHECK_NULL(f, te_free(ret));

        te_expr *prev = ret;
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, f);
        CHECK_NULL(ret, te_free(f), te_free(prev));

        ret->function = t;
    }

    return ret;
}


static te_expr *expr(state *s) {
    /* <expr>      =    <term> {("+" | "-") <term>} */
    te_expr *ret = term(s);
    CHECK_NULL(ret);

    while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
        te_fun2 t = s->function;
        next_token(s);
        te_expr *te = term(s);
        CHECK_NULL(te, te_free(ret));

        te_expr *prev = ret;
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, te);
        CHECK_NULL(ret, te_free(te), te_free(prev));

        ret->function = t;
    }

    return ret;
}


static te_expr *list(state *s) {
    /* <list>      =    <expr> {"," <expr>} */
    te_expr *ret = expr(s);
    CHECK_NULL(ret);

    while (s->type == TOK_SEP) {
        next_token(s);
        te_expr *e = expr(s);
        CHECK_NULL(e, te_free(ret));

        te_expr *prev = ret;
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, e);
        CHECK_NULL(ret, te_free(e), te_free(prev));

        ret->function = comma;
    }

    return ret;
}


#define TE_FUN(...) ((double(*)(__VA_ARGS__))n->function)
#define M(e) te_eval(n->parameters[e])


double te_eval(const te_expr *n) {
    if (!n) return NAN;

    switch(TYPE_MASK(n->type)) {
        case TE_CONSTANT: return n->value;
        case TE_VARIABLE: return *n->bound;

        case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
        case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
            switch(ARITY(n->type)) {
                case 0: return TE_FUN(void)();
                case 1: return TE_FUN(double)(M(0));
                case 2: return TE_FUN(double, double)(M(0), M(1));
                case 3: return TE_FUN(double, double, double)(M(0), M(1), M(2));
                case 4: return TE_FUN(double, double, double, double)(M(0), M(1), M(2), M(3));
                case 5: return TE_FUN(double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4));
                case 6: return TE_FUN(double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5));
                case 7: return TE_FUN(double, double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                default: return NAN;
            }

        case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
        case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
            switch(ARITY(n->type)) {
                case 0: return TE_FUN(void*)(n->parameters[0]);
                case 1: return TE_FUN(void*, double)(n->parameters[1], M(0));
                case 2: return TE_FUN(void*, double, double)(n->parameters[2], M(0), M(1));
                case 3: return TE_FUN(void*, double, double, double)(n->parameters[3], M(0), M(1), M(2));
                case 4: return TE_FUN(void*, double, double, double, double)(n->parameters[4], M(0), M(1), M(2), M(3));
                case 5: return TE_FUN(void*, double, double, double, double, double)(n->parameters[5], M(0), M(1), M(2), M(3), M(4));
                case 6: return TE_FUN(void*, double, double, double, double, double, double)(n->parameters[6], M(0), M(1), M(2), M(3), M(4), M(5));
                case 7: return TE_FUN(void*, double, double, double, double, double, double, double)(n->parameters[7], M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                default: return NAN;
            }

        default: return NAN;
    }

}

#undef TE_FUN
#undef M

static void optimize(te_expr *n) {
    /* Evaluates as much as possible. */
    if (n->type == TE_CONSTANT) return;
    if (n->type == TE_VARIABLE) return;

    /* Only optimize out functions flagged as pure. */
    if (IS_PURE(n->type)) {
        const int arity = ARITY(n->type);
        int known = 1;
        int i;
        for (i = 0; i < arity; ++i) {
            optimize(n->parameters[i]);
            if (((te_expr*)(n->parameters[i]))->type != TE_CONSTANT) {
                known = 0;
            }
        }
        if (known) {
            const double value = te_eval(n);
            te_free_parameters(n);
            n->type = TE_CONSTANT;
            n->value = value;
        }
    }
}


te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {
    state s;
    s.start = s.next = expression;
    s.lookup = variables;
    s.lookup_len = var_count;

    next_token(&s);
    te_expr *root = list(&s);
    if (root == NULL) {
        if (error) *error = -1;
        return NULL;
    }

    if (s.type != TOK_END) {
        te_free(root);
        if (error) {
            *error = (s.next - s.start);
            if (*error == 0) *error = 1;
        }
        return 0;
    } else {
        optimize(root);
        if (error) *error = 0;
        return root;
    }
}


double te_interp(const char *expression, int *error) {
    te_expr *n = te_compile(expression, 0, 0, error);

    double ret;
    if (n) {
        ret = te_eval(n);
        te_free(n);
    } else {
        ret = NAN;
    }
    return ret;
}

static void pn (const te_expr *n, int depth) {
    int i, arity;
    //printf("%*s", depth, "");

    switch(TYPE_MASK(n->type)) {
    //case TE_CONSTANT: printf("%f\n", n->value); break;
    //case TE_VARIABLE: printf("bound %p\n", n->bound); break;
    case TE_CONSTANT: break;
    case TE_VARIABLE: break;

    case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
    case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
    case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
    case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
         arity = ARITY(n->type);
         //printf("f%d", arity);
         for(i = 0; i < arity; i++) {
             //printf(" %p", n->parameters[i]);
         }
         //printf("\n");
         for(i = 0; i < arity; i++) {
             pn(n->parameters[i], depth + 1);
         }
         break;
    }
}


void te_print(const te_expr *n) {
    pn(n, 0);
}
