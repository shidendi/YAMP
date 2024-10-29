#include "LexicalAnalyzer.h"
#include "TokenTypes.h"

bool LexicalAnalyzer::checkIfOperator(const std::string& operatorStr) const {
    return (operatorStr == "+" || operatorStr == "-" || operatorStr == "=" || operatorStr == "==" || operatorStr == "!=");
}

bool LexicalAnalyzer::checkIfDelimiter(char character) const {
    return (character == '(' || character == ')' || character == '{' || character == '}' || character == ',' || character == ';');
}

void LexicalAnalyzer::processOperatorToken(char operatorChar) {
    std::string operatorStr(1, operatorChar);
    char nextChar;
    if (operatorChar == '=' || operatorChar == '!' || operatorChar == '<' || operatorChar == '>') {
        if (inputFile.get(nextChar) && nextChar == '=') {
            operatorStr += nextChar;
        }
        else {
            inputFile.unget();
        }
    }
    tokenList.appendToken(Token(TokenType::OPERATOR, operatorStr, -1));
}

void LexicalAnalyzer::processDelimiterToken(char delimiterChar) {
    std::string delimStr(1, delimiterChar);
    tokenList.appendToken(Token(TokenType::DELIMITER, delimStr, -1));
}
