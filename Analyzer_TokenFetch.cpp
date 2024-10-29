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

        // Проверка на разделители
        if (checkIfDelimiter(character)) {
            processDelimiterToken(character);
            return Token(TokenType::DELIMITER, std::string(1, character), index); // Возвращаем токен разделителя
        }

        // Проверка на двухсимвольные операторы в первую очередь
        if (character == '=' || character == '!' || character == '<' || character == '>') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '=') {
                return Token(TokenType::OPERATOR, std::string(1, character) + nextChar, index); // Например, '==', '!=', '<=', '>='
            }
            else {
                inputFile.unget(); // Возвращаем символ обратно, если это не '='
                return Token(TokenType::OPERATOR, std::string(1, character), index); // Возвращаем односимвольный оператор
            }
        }

        // Проверяем односимвольные операторы
        if (checkIfOperator(std::string(1, character))) {
            return Token(TokenType::OPERATOR, std::string(1, character), index);
        }

        // Проверка на идентификаторы, ключевые слова или ошибочные лексемы
        if (isalpha(character)) { // Первый символ должен быть буквой
            lexeme.clear();
            lexeme += character;
            bool isError = false;

            // Читаем оставшиеся символы идентификатора
            while (inputFile.get(character)) {
                if (isalpha(character)) { // Остальные символы должны быть только буквами
                    lexeme += character;
                }
                else if (isdigit(character)) {
                    // Если после первого символа-идентификатора встречаются цифры, помечаем как ошибку
                    isError = true;
                    lexeme += character;
                }
                else {
                    if (!isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                        isError = true;  // Обнаружен недопустимый символ внутри идентификатора
                        lexeme += character;
                    }
                    else {
                        inputFile.unget(); // Возвращаем символ обратно, если это допустимый разделитель
                    }
                    break;
                }
            }

            // Если лексема содержит ошибку
            if (isError) {
                while (inputFile.get(character) && !isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                    lexeme += character;
                }
                inputFile.unget();
                return Token(TokenType::ERROR, lexeme, index);
            }
            else {
                TokenType type = classifyIdentifier(lexeme);
                return Token(type, lexeme, index);
            }
        }

        // Проверка на числа и возможные ошибки после числа
        else if (isdigit(character)) {
            lexeme.clear();
            lexeme += character;
            bool isError = false;

            // Читаем оставшиеся символы числа
            while (inputFile.get(character)) {
                if (isdigit(character)) {
                    lexeme += character;
                }
                else if (isalpha(character) || character == '_') {
                    isError = true;  // Если буква или символ после цифры, помечаем как ошибку
                    lexeme += character;
                }
                else if (!isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                    isError = true;  // Любой другой недопустимый символ после числа
                    lexeme += character;
                }
                else {
                    inputFile.unget(); // Возвращаем символ обратно, если не алфавитно-цифровой символ
                    break;
                }
            }

            // Если число содержит ошибку
            if (isError) {
                while (inputFile.get(character) && !isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                    lexeme += character;
                }
                inputFile.unget();
                return Token(TokenType::ERROR, lexeme, index);
            }
            else {
                return Token(TokenType::INT_NUM, lexeme, index);
            }
        }

        // Обработка лексем, начинающихся с недопустимого символа
        else {
            lexeme.clear();
            lexeme += character;

            // Читаем оставшиеся символы ошибки
            while (inputFile.get(character) && !isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                lexeme += character;
            }
            inputFile.unget();

            return Token(TokenType::ERROR, lexeme, index); // Лексема содержит недопустимые символы
        }
    }

    // Возвращаем токен неизвестного типа, если достигли конца файла
    return Token(TokenType::UNKNOWN, "", index);
}
