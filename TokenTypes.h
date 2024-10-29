#ifndef TOKENTYPES_H
#define TOKENTYPES_H

#include <string>
#include <fstream>

enum class TokenType {
    TYPE,           // int
    ID_NAME,        // идентификаторы (имена переменных и функций)
    INT_NUM,        // целые числавот
    OPERATOR,       // операторы (+, -, =)
    DELIMITER,      // разделители ((), {}, ,, ;)
    RETURN,         // return
    OP,             // for
    ERROR,          // ошибки или неизвестные символы
    UNKNOWN         // неизвестные токены (например, конец файла)
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    int index;
    Token() : type(TokenType::UNKNOWN), lexeme(""), index(-1) {}

    Token(TokenType t, const std::string& lex, int idx) : type(t), lexeme(lex), index(idx) {}
};

#endif
