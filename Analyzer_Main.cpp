#include "LexicalAnalyzer.h"
#include "TokenTypes.h"

LexicalAnalyzer::LexicalAnalyzer(const std::string& inputFileName, const std::string& outputFileName)
    : inputFile(inputFileName), outputFile(outputFileName) {
    // Проверяем, открылись ли файлы успешно
    if (!inputFile.is_open()) {
        throw std::runtime_error("Error opening input file: " + inputFileName);
    }
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error opening output file: " + outputFileName);
    }
}

LexicalAnalyzer::~LexicalAnalyzer() {
    // Закрываем файлы автоматически, если они открыты
    if (inputFile.is_open()) inputFile.close();
    if (outputFile.is_open()) inputFile.close();
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
            return TokenType::RETURN; // "return" распознано
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
            operatorStr += nextChar; // Оператор двухсимвольный (например, '==', '!=')
        }
        else {
            inputFile.unget(); // Возвращаем символ обратно, если не '='
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
            break;  // Прерываем цикл, если достигнут конец файла
        }
        tokenList.appendToken(token);
    }

    // Печатаем все токены, за исключением токенов с ошибками, в выходной файл
    tokenList.writeTokensToFile(outputFile);
}
