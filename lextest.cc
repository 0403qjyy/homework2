/*
 * lextest.cc - 测试COOL词法分析器的主程序
 */

#include <cool-parse.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

// 声明全局变量
extern FILE *yyin;
extern int cool_yylex(void);
extern int curr_lineno;
extern char *curr_filename;
extern YYSTYPE cool_yylval;

void usage(const char *program_name) {
    std::cerr << "Usage: " << program_name << " [filename]" << std::endl;
    std::cerr << "  If filename is provided, read from file." << std::endl;
    std::cerr << "  Otherwise, read from standard input." << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    // 处理命令行参数
    if (argc > 2) {
        usage(argv[0]);
    }

    // 设置输入文件
    if (argc == 2) {
        // 从文件读取
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            std::cerr << "Error: Cannot open file '" << argv[1] << "'" << std::endl;
            return 1;
        }
        curr_filename = argv[1];
    } else {
        // 从标准输入读取
        yyin = stdin;
        curr_filename = "(stdin)";
    }

    // 打印文件名
    std::cout << "#name \"" << curr_filename << "\"" << std::endl;

    // 重置行号
    curr_lineno = 1;

    int token;
    while ((token = cool_yylex()) != 0) {
        // 打印token信息
        switch (token) {
            case CLASS:
                std::cout << "#" << curr_lineno << " CLASS" << std::endl;
                break;
            case ELSE:
                std::cout << "#" << curr_lineno << " ELSE" << std::endl;
                break;
            case FI:
                std::cout << "#" << curr_lineno << " FI" << std::endl;
                break;
            case IF:
                std::cout << "#" << curr_lineno << " IF" << std::endl;
                break;
            case IN:
                std::cout << "#" << curr_lineno << " IN" << std::endl;
                break;
            case INHERITS:
                std::cout << "#" << curr_lineno << " INHERITS" << std::endl;
                break;
            case ISVOID:
                std::cout << "#" << curr_lineno << " ISVOID" << std::endl;
                break;
            case LET:
                std::cout << "#" << curr_lineno << " LET" << std::endl;
                break;
            case LOOP:
                std::cout << "#" << curr_lineno << " LOOP" << std::endl;
                break;
            case POOL:
                std::cout << "#" << curr_lineno << " POOL" << std::endl;
                break;
            case THEN:
                std::cout << "#" << curr_lineno << " THEN" << std::endl;
                break;
            case WHILE:
                std::cout << "#" << curr_lineno << " WHILE" << std::endl;
                break;
            case CASE:
                std::cout << "#" << curr_lineno << " CASE" << std::endl;
                break;
            case ESAC:
                std::cout << "#" << curr_lineno << " ESAC" << std::endl;
                break;
            case NEW:
                std::cout << "#" << curr_lineno << " NEW" << std::endl;
                break;
            case OF:
                std::cout << "#" << curr_lineno << " OF" << std::endl;
                break;
            case NOT:
                std::cout << "#" << curr_lineno << " NOT" << std::endl;
                break;
            case BOOL_CONST:
                std::cout << "#" << curr_lineno << " BOOL_CONST " 
                         << (cool_yylval.boolean ? "true" : "false") << std::endl;
                break;
            case TYPEID:
                if (cool_yylval.symbol) {
                    std::cout << "#" << curr_lineno << " TYPEID " 
                             << cool_yylval.symbol->get_string() << std::endl;
                } else {
                    std::cout << "#" << curr_lineno << " TYPEID <null>" << std::endl;
                }
                break;
            case OBJECTID:
                if (cool_yylval.symbol) {
                    std::cout << "#" << curr_lineno << " OBJECTID " 
                             << cool_yylval.symbol->get_string() << std::endl;
                } else {
                    std::cout << "#" << curr_lineno << " OBJECTID <null>" << std::endl;
                }
                break;
            case INT_CONST:
                if (cool_yylval.symbol) {
                    std::cout << "#" << curr_lineno << " INT_CONST " 
                             << cool_yylval.symbol->get_string() << std::endl;
                } else {
                    std::cout << "#" << curr_lineno << " INT_CONST <null>" << std::endl;
                }
                break;
            case STR_CONST:
                if (cool_yylval.symbol) {
                    std::cout << "#" << curr_lineno << " STR_CONST " 
                             << cool_yylval.symbol->get_string() << std::endl;
                } else {
                    std::cout << "#" << curr_lineno << " STR_CONST <null>" << std::endl;
                }
                break;
            case DARROW:
                std::cout << "#" << curr_lineno << " DARROW" << std::endl;
                break;
            case ASSIGN:
                std::cout << "#" << curr_lineno << " ASSIGN" << std::endl;
                break;
            case LE:
                std::cout << "#" << curr_lineno << " LE" << std::endl;
                break;
            case ERROR:
                if (cool_yylval.error_msg) {
                    std::cout << "#" << curr_lineno << " ERROR " 
                             << cool_yylval.error_msg << std::endl;
                } else {
                    std::cout << "#" << curr_lineno << " ERROR <unknown>" << std::endl;
                }
                break;
            case '+':
                std::cout << "#" << curr_lineno << " '+' " << std::endl;
                break;
            case '-':
                std::cout << "#" << curr_lineno << " '-' " << std::endl;
                break;
            case '*':
                std::cout << "#" << curr_lineno << " '*' " << std::endl;
                break;
            case '/':
                std::cout << "#" << curr_lineno << " '/' " << std::endl;
                break;
            case '~':
                std::cout << "#" << curr_lineno << " '~' " << std::endl;
                break;
            case '<':
                std::cout << "#" << curr_lineno << " '<' " << std::endl;
                break;
            case '=':
                std::cout << "#" << curr_lineno << " '=' " << std::endl;
                break;
            case '.':
                std::cout << "#" << curr_lineno << " '.' " << std::endl;
                break;
            case '@':
                std::cout << "#" << curr_lineno << " '@' " << std::endl;
                break;
            case ',':
                std::cout << "#" << curr_lineno << " ',' " << std::endl;
                break;
            case ';':
                std::cout << "#" << curr_lineno << " ';' " << std::endl;
                break;
            case ':':
                std::cout << "#" << curr_lineno << " ':' " << std::endl;
                break;
            case '(':
                std::cout << "#" << curr_lineno << " '(' " << std::endl;
                break;
            case ')':
                std::cout << "#" << curr_lineno << " ')' " << std::endl;
                break;
            case '{':
                std::cout << "#" << curr_lineno << " '{' " << std::endl;
                break;
            case '}':
                std::cout << "#" << curr_lineno << " '}' " << std::endl;
                break;
            default:
                std::cout << "#" << curr_lineno << " UNKNOWN_TOKEN " << token << std::endl;
                break;
        }

        // 遇到错误时停止
        if (token == ERROR) {
            break;
        }
    }

    // 关闭文件（如果不是stdin）
    if (argc == 2) {
        fclose(yyin);
    }

    return 0;
}
