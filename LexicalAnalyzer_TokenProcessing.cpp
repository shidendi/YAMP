#include "LexicalAnalyzer.h"
#include "TokenTypes.h"




Token LexicalAnalyzer::fetchNextToken() {
    std::string lexeme;
    char character;
    int index = 1;

    while (inputFile.get(character)) {
        if (isspace(character)) {
            index++;
            continue;
        }

        // Пропуск комментариев (начинающихся с //)
        if (character == '/') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '/') {
                // Пропускаем символы до конца строки, поскольку это комментарий
                while (inputFile.get(character) && character != '\n') {
                    // Пропускаем все символы до конца строки
                }
                continue; // Переходим к следующей итерации, чтобы пропустить комментарий
            }
            else {
                inputFile.unget(); // Возвращаем символ обратно, если это не комментарий
            }
        }

        // Проверка на двухсимвольные операторы
        if (character == '=' || character == '!' || character == '<' || character == '>') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '=') {
                return Token(TokenType::OPERATOR, std::string(1, character) + nextChar, index); // Например, '==', '!='
            }
            else {
                inputFile.unget();
                return Token(TokenType::OPERATOR, std::string(1, character), index);
            }
        }

        // Проверка на идентификаторы и числа
        if (isalpha(character)) {
            lexeme.clear();
            lexeme += character;
            while (inputFile.get(character) && (isalpha(character) || isdigit(character))) {
                lexeme += character;
            }
            inputFile.unget();
            return Token(classifyIdentifier(lexeme), lexeme, index);
        }
        else if (isdigit(character)) {
            lexeme.clear();
            lexeme += character;
            while (inputFile.get(character) && isdigit(character)) {
                lexeme += character;
            }
            inputFile.unget();
            return Token(TokenType::INT_NUM, lexeme, index);
        }

        lexeme.clear();
        lexeme += character;
        return Token(TokenType::ERROR, lexeme, index);
    }

    return Token(TokenType::UNKNOWN, "", index);
}

TokenType LexicalAnalyzer::classifyIdentifier(const std::string& lexeme) {
    // Проверяем, является ли лексема ключевым словом, сравнивая символы
    size_t pos = 0;

    // Проверка на "int"
    if (lexeme.length() >= 3 && lexeme[0] == 'i' && lexeme[1] == 'n' && lexeme[2] == 't') {
        // Проверяем, что после ключевого слова следует разделитель
        if (lexeme.length() == 3 || checkIfDelimiter(lexeme[3]) || isspace(lexeme[3])) {
            return TokenType::TYPE; // "int" распознано
        }
    }

    // Проверка на "return"
    if (lexeme.length() >= 6 && lexeme[0] == 'r' && lexeme[1] == 'e' && lexeme[2] == 't' &&
        lexeme[3] == 'u' && lexeme[4] == 'r' && lexeme[5] == 'n') {
        // Проверяем, что после ключевого слова следует разделитель
        if (lexeme.length() == 6 || checkIfDelimiter(lexeme[6]) || isspace(lexeme[6])) {
            return TokenType::END; // "return" распознано
        }
    }

    // Проверка на "for"
    if (lexeme.length() >= 3 && lexeme[0] == 'f' && lexeme[1] == 'o' && lexeme[2] == 'r') {
        // Проверяем, что после ключевого слова следует разделитель
        if (lexeme.length() == 3 || checkIfDelimiter(lexeme[3]) || isspace(lexeme[3])) {
            return TokenType::OP; // "for" распознано
        }
    }

    // Если ни одно ключевое слово не распознано, возвращаем как идентификатор
    return TokenType::ID_NAME;
}

TokenType LexicalAnalyzer::classifyNumber(const std::string& lexeme) {
    return TokenType::INT_NUM;
}
