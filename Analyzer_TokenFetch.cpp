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

        // ������� ������������ (������������ � //)
        if (character == '/') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '/') {
                // ���������� ������� �� ����� ������, ��������� ��� �����������
                while (inputFile.get(character) && character != '\n') {
                    // ���������� ��� ������� �� ����� ������
                }
                continue; // ��������� � ��������� ��������, ����� ���������� �����������
            }
            else {
                inputFile.unget(); // ���������� ������ �������, ���� ��� �� �����������
            }
        }

        // �������� �� �����������
        if (checkIfDelimiter(character)) {
            processDelimiterToken(character);
            return Token(TokenType::DELIMITER, std::string(1, character), index); // ���������� ����� �����������
        }

        // �������� �� �������������� ��������� � ������ �������
        if (character == '=' || character == '!' || character == '<' || character == '>') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '=') {
                return Token(TokenType::OPERATOR, std::string(1, character) + nextChar, index); // ��������, '==', '!=', '<=', '>='
            }
            else {
                inputFile.unget(); // ���������� ������ �������, ���� ��� �� '='
                return Token(TokenType::OPERATOR, std::string(1, character), index); // ���������� �������������� ��������
            }
        }

        // ��������� �������������� ���������
        if (checkIfOperator(std::string(1, character))) {
            return Token(TokenType::OPERATOR, std::string(1, character), index);
        }

        // �������� �� ��������������, �������� ����� ��� ��������� �������
        if (isalpha(character)) { // ������ ������ ������ ���� ������
            lexeme.clear();
            lexeme += character;
            bool isError = false;

            // ������ ���������� ������� ��������������
            while (inputFile.get(character)) {
                if (isalpha(character)) { // ��������� ������� ������ ���� ������ �������
                    lexeme += character;
                }
                else if (isdigit(character)) {
                    // ���� ����� ������� �������-�������������� ����������� �����, �������� ��� ������
                    isError = true;
                    lexeme += character;
                }
                else {
                    if (!isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                        isError = true;  // ��������� ������������ ������ ������ ��������������
                        lexeme += character;
                    }
                    else {
                        inputFile.unget(); // ���������� ������ �������, ���� ��� ���������� �����������
                    }
                    break;
                }
            }

            // ���� ������� �������� ������
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

        // �������� �� ����� � ��������� ������ ����� �����
        else if (isdigit(character)) {
            lexeme.clear();
            lexeme += character;
            bool isError = false;

            // ������ ���������� ������� �����
            while (inputFile.get(character)) {
                if (isdigit(character)) {
                    lexeme += character;
                }
                else if (isalpha(character) || character == '_') {
                    isError = true;  // ���� ����� ��� ������ ����� �����, �������� ��� ������
                    lexeme += character;
                }
                else if (!isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                    isError = true;  // ����� ������ ������������ ������ ����� �����
                    lexeme += character;
                }
                else {
                    inputFile.unget(); // ���������� ������ �������, ���� �� ���������-�������� ������
                    break;
                }
            }

            // ���� ����� �������� ������
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

        // ��������� ������, ������������ � ������������� �������
        else {
            lexeme.clear();
            lexeme += character;

            // ������ ���������� ������� ������
            while (inputFile.get(character) && !isspace(character) && !checkIfDelimiter(character) && !checkIfOperator(std::string(1, character))) {
                lexeme += character;
            }
            inputFile.unget();

            return Token(TokenType::ERROR, lexeme, index); // ������� �������� ������������ �������
        }
    }

    // ���������� ����� ������������ ����, ���� �������� ����� �����
    return Token(TokenType::UNKNOWN, "", index);
}
