#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <string>
#include "TokenContainer.h"

class LexicalAnalyzer {
public:

    // Конструктор принимает два имени файлов: один для входных данных, другой для выходны
    LexicalAnalyzer(const std::string& inputFileName, const std::string& outputFileName);

    // Деструктор, закрывающий файлы и освобождающий ресурсы
    ~LexicalAnalyzer();
    void analyze();

private:
    std::ifstream inputFile;
    std::ofstream outputFile;
    TokenList tokenList;


    // Методы для классификации лексем

    // Определение типа идентификатора (ключевое слово или имя переменной)
    TokenType classifyIdentifier(const std::string& lexeme);

    // Определение типа числа (целое число)
    TokenType classifyNumber(const std::string& lexeme);

    // Проверка, является ли строка оператором
    bool checkIfOperator(const std::string& str) const;

    // Проверка, является ли символ разделителем
    bool checkIfDelimiter(char c) const;

    // Обработка токена, если он является оператором
    void processOperatorToken(char c);

    // Обработка токена, если он является разделителем
    void processDelimiterToken(char c);

    // Извлечение следующего токена из входного файла
    Token fetchNextToken();
};

#endif