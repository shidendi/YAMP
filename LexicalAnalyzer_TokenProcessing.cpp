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

        // �������� �� �������������� ���������
        if (character == '=' || character == '!' || character == '<' || character == '>') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '=') {
                return Token(TokenType::OPERATOR, std::string(1, character) + nextChar, index); // ��������, '==', '!='
            }
            else {
                inputFile.unget();
                return Token(TokenType::OPERATOR, std::string(1, character), index);
            }
        }

        // �������� �� �������������� � �����
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
    // ���������, �������� �� ������� �������� ������, ��������� �������
    size_t pos = 0;

    // �������� �� "int"
    if (lexeme.length() >= 3 && lexeme[0] == 'i' && lexeme[1] == 'n' && lexeme[2] == 't') {
        // ���������, ��� ����� ��������� ����� ������� �����������
        if (lexeme.length() == 3 || checkIfDelimiter(lexeme[3]) || isspace(lexeme[3])) {
            return TokenType::TYPE; // "int" ����������
        }
    }

    // �������� �� "return"
    if (lexeme.length() >= 6 && lexeme[0] == 'r' && lexeme[1] == 'e' && lexeme[2] == 't' &&
        lexeme[3] == 'u' && lexeme[4] == 'r' && lexeme[5] == 'n') {
        // ���������, ��� ����� ��������� ����� ������� �����������
        if (lexeme.length() == 6 || checkIfDelimiter(lexeme[6]) || isspace(lexeme[6])) {
            return TokenType::END; // "return" ����������
        }
    }

    // �������� �� "for"
    if (lexeme.length() >= 3 && lexeme[0] == 'f' && lexeme[1] == 'o' && lexeme[2] == 'r') {
        // ���������, ��� ����� ��������� ����� ������� �����������
        if (lexeme.length() == 3 || checkIfDelimiter(lexeme[3]) || isspace(lexeme[3])) {
            return TokenType::OP; // "for" ����������
        }
    }

    // ���� �� ���� �������� ����� �� ����������, ���������� ��� �������������
    return TokenType::ID_NAME;
}

TokenType LexicalAnalyzer::classifyNumber(const std::string& lexeme) {
    return TokenType::INT_NUM;
}
