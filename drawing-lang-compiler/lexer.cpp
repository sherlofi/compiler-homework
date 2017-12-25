#include <iostream>
#include "lexer.hpp"
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

static ifstream ExampleFile;
unsigned int LineNo;

int InitScanner(const char *FileName) {
    LineNo = 1;
    ExampleFile.open(FileName);
    if (ExampleFile.is_open()) {
        cout << "[lexer]  Open Success" << endl;
        return 0;
    } else {
        cout << "[lexer]  Open Failed" << endl;
        return 1;
    }
}

void CloseScanner(void) {
    if (ExampleFile.is_open())
        ExampleFile.close();
}

static Token CheckInTable(const string String) {
    for (int loop = 0; loop < sizeof(TokenTable) / sizeof(Token); loop++) {
        if (String.compare(TokenTable[loop].lexeme) == 0)
            return TokenTable[loop];
    }
    Token errortoken;
    errortoken = {ERRTOKEN, "", 0.0, NULL};
    return errortoken;
}

static char GetChar(void) {
    char Char = ExampleFile.get();
    return toupper(Char);
}

Token GetToken(void) {
    Token token = {ERRTOKEN, "", 0.0, NULL};
    char Char;

    while (true) {
        Char = GetChar();
        if (Char == EOF) {
            token.type = NONTOKEN;
            return token;
        }
        if (Char == '\n')
            LineNo++;
        if (!isspace(Char))
            break;
    }

    token.lexeme += Char;

    if (isalpha(Char)) {
        while (true) {
            Char = GetChar();
            if (isalnum(Char))
                token.lexeme += Char;
            else
                break;
        }
        ExampleFile.unget();
        token = CheckInTable(token.lexeme);
        return token;
    } else if (isdigit(Char)) {
        while (true) {
            Char = GetChar();
            if (isdigit(Char))
                token.lexeme += Char;
            else
                break;
        }
        if (Char == '.') {
            token.lexeme += Char;
            while (true) {
                Char = GetChar();
                if (isdigit(Char))
                    token.lexeme += Char;
                else
                    break;
            }
        }
        ExampleFile.unget();
        token.type = CONST_ID;
        token.value = atof(token.lexeme.c_str());
    } else {
        switch (Char) {
            case ';':
                token.type = SEMICO;
                break;
            case '(':
                token.type = L_BRACKET;
                break;
            case ')':
                token.type = R_BRACKET;
                break;
            case ',':
                token.type = COMMA;
                break;
            case '+':
                token.type = PLUS;
                break;
            case '-':
                Char = GetChar();
                if (Char == '-') {
                    while (Char != '\n' && Char != EOF)
                        Char = GetChar();
                    return GetToken();
                } else {
                    ExampleFile.unget();
                    token.type = MINUS;
                    break;
                }
            case '/':
                Char = GetChar();
                if (Char == '/') {
                    while (Char != '\n' && Char != EOF)
                        Char = GetChar();
                    return GetToken();
                } else {
                    ExampleFile.unget();
                    token.type = DIV;
                    break;
                }
            case '*':
                Char = GetChar();
                if (Char == '*') {
                    token.type = POWER;
                    break;
                } else {
                    ExampleFile.unget();
                    token.type = MUL;
                    break;
                }
            default:
                token.type = ERRTOKEN;
                break;
        } // end of switch
    } // end of else
    return token;
}




















