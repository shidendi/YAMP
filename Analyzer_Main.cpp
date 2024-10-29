#include "LexicalAnalyzer.h"
#include "TokenTypes.h"

LexicalAnalyzer::LexicalAnalyzer(const std::string& inputFileName, const std::string& outputFileName)
    : inputFile(inputFileName), outputFile(outputFileName) {
    // ���������, ��������� �� ����� �������
    if (!inputFile.is_open()) {
        throw std::runtime_error("Error opening input file: " + inputFileName);
    }
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error opening output file: " + outputFileName);
    }
}

LexicalAnalyzer::~LexicalAnalyzer() {
    // ��������� ����� �������������, ���� ��� �������
    if (inputFile.is_open()) inputFile.close();
    if (outputFile.is_open()) inputFile.close();
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
            return TokenType::RETURN; // "return" ����������
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

bool LexicalAnalyzer::checkIfOperator(const std::string& operatorStr) const {
    return (operatorStr == "+" || operatorStr == "-" || operatorStr == "*" || operatorStr == "/" ||
        operatorStr == "=" || operatorStr == "==" || operatorStr == "!=" || operatorStr == "<" || operatorStr == ">" ||
        operatorStr == "<=" || operatorStr == ">=" || operatorStr == ";" || operatorStr == "(" || operatorStr == ")" ||
        operatorStr == "{" || operatorStr == "}" || operatorStr == ",");
}

TokenType LexicalAnalyzer::classifyNumber(const std::string& lexeme) {
    return TokenType::INT_NUM;
}

bool LexicalAnalyzer::checkIfDelimiter(char character) const {
    return (character == '(' || character == ')' || character == '{' || character == '}' || character == ',' || character == ';' || character == '\'' || character == '\"' || character == '.');
}

void LexicalAnalyzer::processOperatorToken(char operatorChar) {
    std::string operatorStr(1, operatorChar);
    char nextChar;
    if (operatorChar == '=' || operatorChar == '!' || operatorChar == '<' || operatorChar == '>') {
        if (inputFile.get(nextChar) && nextChar == '=') {
            operatorStr += nextChar; // �������� �������������� (��������, '==', '!=')
        }
        else {
            inputFile.unget(); // ���������� ������ �������, ���� �� '='
        }
    }

    tokenList.appendToken(Token(TokenType::OPERATOR, operatorStr, -1));
}

void LexicalAnalyzer::processDelimiterToken(char delimiterChar) {
    std::string delimStr(1, delimiterChar);
    tokenList.appendToken(Token(TokenType::DELIMITER, delimStr, -1));
}

void LexicalAnalyzer::analyze() {
    while (true) {
        Token token = fetchNextToken();
        if (token.type == TokenType::UNKNOWN) {
            break;  // ��������� ����, ���� ��������� ����� �����
        }
        tokenList.appendToken(token);
    }

    // �������� ��� ������, �� ����������� ������� � ��������, � �������� ����
    tokenList.writeTokensToFile(outputFile);
}
