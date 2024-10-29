#ifndef TOKENLIST_H
#define TOKENLIST_H

#include "TokenTypes.h"
#include <fstream>

class TokenList {
public:
    TokenList();
    ~TokenList();

    void appendToken(const Token& token);
    int generateHash(const std::string& lexeme) const;

    // Объявление функции вывода токенов
    void writeTokensToFile(std::ofstream& outputFile);

    // Метод для очистки списка токенов
    void clearTokens();

    // Метод для получения количества токенов
    int getTokenCount() const;

private:
    static const int HASH_TABLE_SIZE = 1000;
    Token* hashTable[HASH_TABLE_SIZE];

    // Массив для хранения токенов в порядке их появления
    static const int MAX_TOKENS = 10000; // Максимальное количество токенов
    Token* tokenSequence[MAX_TOKENS];
    int tokenCount; // Счетчик количества добавленных токенов
};

#endif // TOKENLIST_H
