%{
/*
 * cool.flex - COOL Lexer Specification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cool-parse.h"
#include "stringtab.h"
#include "utilities.h"

/* 定义全局变量 */
int curr_lineno = 1;
int cool_yydebug = 0;
YYSTYPE cool_yylval;
FILE *fin = stdin;
char *curr_filename = "(stdin)";

/* 函数重命名 */
#define yylex cool_yylex

/* 字符串缓冲区 */
#define MAX_STR_CONST 1024
char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

/* 注释嵌套计数 */
static int comment_level = 0;

%}

%option noyywrap
%option nodefault

%x COMMENT
%x STRING

DARROW          =>
ASSIGN          <-
LE              <=

DIGIT           [0-9]
LETTER          [a-zA-Z]
UPPER           [A-Z]
LOWER           [a-z]
ALNUM           [a-zA-Z0-9]
WHITESPACE      [ \t\r\f\v]

%%

[cC][lL][aA][sS][sS]       { return CLASS; }
[eE][lL][sS][eE]           { return ELSE; }
[fF][iI]                   { return FI; }
[iI][fF]                   { return IF; }
[iI][nN]                   { return IN; }
[iI][nN][hH][eE][rR][iI][tT][sS] { return INHERITS; }
[iI][sS][vV][oO][iI][dD]   { return ISVOID; }
[lL][eE][tT]               { return LET; }
[lL][oO][oO][pP]           { return LOOP; }
[pP][oO][oO][lL]           { return POOL; }
[tT][hH][eE][nN]           { return THEN; }
[wW][hH][iI][lL][eE]       { return WHILE; }
[cC][aA][sS][eE]           { return CASE; }
[eE][sS][aA][cC]           { return ESAC; }
[nN][eE][wW]               { return NEW; }
[oO][fF]                   { return OF; }
[nN][oO][tT]               { return NOT; }

t[rR][uU][eE]              { cool_yylval.boolean = 1; return BOOL_CONST; }
f[aA][lL][sS][eE]          { cool_yylval.boolean = 0; return BOOL_CONST; }

{UPPER}({ALNUM}|_)*        { 
    cool_yylval.symbol = stringtable.add_string(yytext); 
    return TYPEID; 
}

{LOWER}({ALNUM}|_)*        { 
    cool_yylval.symbol = stringtable.add_string(yytext); 
    return OBJECTID; 
}

{DIGIT}+                   { 
    cool_yylval.symbol = stringtable.add_string(yytext); 
    return INT_CONST; 
}

{DARROW}                    { return DARROW; }
{ASSIGN}                    { return ASSIGN; }
{LE}                        { return LE; }

"+"                         { return '+'; }
"-"                         { return '-'; }
"*"                         { return '*'; }
"/"                         { return '/'; }
"~"                         { return '~'; }
"<"                         { return '<'; }
"="                         { return '='; }
"."                         { return '.'; }
"@"                         { return '@'; }
","                         { return ','; }
";"                         { return ';'; }
":"                         { return ':'; }
"("                         { return '('; }
")"                         { return ')'; }
"{"                         { return '{'; }
"}"                         { return '}'; }

"--".*                      { }

"(*"                        { 
    BEGIN(COMMENT); 
    comment_level = 1; 
}

<COMMENT>"(*"               { comment_level++; }
<COMMENT>"*)"               { 
    comment_level--; 
    if (comment_level == 0) BEGIN(INITIAL); 
}
<COMMENT>\n                 { curr_lineno++; }
<COMMENT><<EOF>>            { 
    cool_yylval.error_msg = "EOF in comment"; 
    BEGIN(INITIAL); 
    return ERROR; 
}
<COMMENT>.                  { }

"*)"                        { 
    cool_yylval.error_msg = "Unmatched *)"; 
    return ERROR; 
}

\"                          { 
    BEGIN(STRING); 
    string_buf_ptr = string_buf; 
}

<STRING>\"                  { 
    BEGIN(INITIAL); 
    *string_buf_ptr = '\0'; 
    
    if (string_buf_ptr - string_buf >= MAX_STR_CONST) {
        cool_yylval.error_msg = "String constant too long";
        return ERROR;
    }
    
    cool_yylval.symbol = stringtable.add_string(string_buf);
    return STR_CONST;
}

<STRING>\n                  { 
    BEGIN(INITIAL); 
    cool_yylval.error_msg = "Unterminated string constant"; 
    return ERROR; 
}

<STRING><<EOF>>             { 
    BEGIN(INITIAL); 
    cool_yylval.error_msg = "EOF in string constant"; 
    return ERROR; 
}

<STRING>\\n                 { 
    if (string_buf_ptr - string_buf < MAX_STR_CONST - 1) {
        *string_buf_ptr++ = '\n'; 
    }
}
<STRING>\\t                 { 
    if (string_buf_ptr - string_buf < MAX_STR_CONST - 1) {
        *string_buf_ptr++ = '\t'; 
    }
}
<STRING>\\b                 { 
    if (string_buf_ptr - string_buf < MAX_STR_CONST - 1) {
        *string_buf_ptr++ = '\b'; 
    }
}
<STRING>\\f                 { 
    if (string_buf_ptr - string_buf < MAX_STR_CONST - 1) {
        *string_buf_ptr++ = '\f'; 
    }
}

<STRING>\\.                 { 
    if (string_buf_ptr - string_buf < MAX_STR_CONST - 1) {
        *string_buf_ptr++ = yytext[1]; 
    }
}

<STRING>.                   { 
    if (yytext[0] == '\0') {
        cool_yylval.error_msg = "String contains null character"; 
        BEGIN(INITIAL); 
        return ERROR;
    }
    
    if (string_buf_ptr - string_buf >= MAX_STR_CONST - 1) {
        cool_yylval.error_msg = "String constant too long"; 
        BEGIN(INITIAL); 
        return ERROR;
    }
    
    *string_buf_ptr++ = yytext[0];
}

{WHITESPACE}+               { }
\n                          { curr_lineno++; }

.                           { 
    cool_yylval.error_msg = yytext; 
    return ERROR; 
}
