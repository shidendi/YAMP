#include "TokenContainer.h"
#include <iostream>
#include <iomanip>
// хэш-таблица - массив указателей на объекты класса токен

// 

TokenList::TokenList() : tokenCount(0) {
    // Инициализация хеш-таблицы
    for (auto& entry : hashTable) {
        entry = nullptr; // Устанавливаем указатели в nullptr
    }

    // Инициализация последовательности токенов
    for (auto& token : tokenSequence) {
        token = nullptr; // Устанавливаем указатели в nullptr
    }
}

TokenList::~TokenList() {
    // Освобождение памяти для хеш-таблицы
    for (Token* tokenPtr : hashTable) {
        delete tokenPtr; // Освобождаем каждый токен в хеш-таблице
    }

    // Освобождение памяти для последовательности токенов
    for (Token* tokenPtr : tokenSequence) {
        delete tokenPtr; // Освобождаем каждый токен в последовательности
    }
}

int TokenList::generateHash(const std::string& lexeme) const {
    int hash = 0;
    for (char c : lexeme) {
        hash = (hash * 31 + c) % HASH_TABLE_SIZE; // Пробирование при коллизии
    }
    return hash;
}

void TokenList::appendToken(const Token& newToken) {
    // Проверяем, существует ли уже токен в хеш-таблице
    int index = generateHash(newToken.lexeme);
    while (hashTable[index] != nullptr) {
        if (hashTable[index]->lexeme == newToken.lexeme && hashTable[index]->type == newToken.type) {
            // Если токен уже существует, не добавляем его
            return;
        }
        index = (index + 1) % HASH_TABLE_SIZE; // Пробирование при коллизии
    }

    // Если токен не найден в хеш-таблице, добавляем его в последовательность и хеш-таблицу
    if (tokenCount < MAX_TOKENS) {
        tokenSequence[tokenCount] = new Token(newToken.type, newToken.lexeme, index);
        tokenCount++;
    }

    // Добавляем токен в хеш-таблицу
    hashTable[index] = new Token(newToken.type, newToken.lexeme, index);
}

void TokenList::writeTokensToFile(std::ofstream& outputFile) {
    // Сначала выводим все токены, кроме типа ERROR
    for (int i = 0; i < tokenCount; ++i) {
        Token* currentToken = tokenSequence[i];
        if (currentToken != nullptr && currentToken->type != TokenType::ERROR) {
            std::string tokenType;
            switch (currentToken->type) {
            case TokenType::TYPE:
                tokenType = "TYPE";
                break;
            case TokenType::RETURN:
                tokenType = "RETURN";
                break;
            case TokenType::OP:
                tokenType = "OP";
                break;
            case TokenType::ID_NAME:
                tokenType = "ID_NAME";
                break;
            case TokenType::INT_NUM:
                tokenType = "INT_NUM";
                break;
            case TokenType::OPERATOR:
                tokenType = "OPERATOR";
                break;
            case TokenType::DELIMITER:
                tokenType = "DELIMITER";
                break;
            default:
                tokenType = "UNKNOWN";
                break;
            }
            outputFile << tokenType << " | " << currentToken->lexeme << " | " << currentToken->index << std::endl;
        }
    }

    outputFile << '\n';

    // Вывод ERROR
    for (int i = 0; i < tokenCount; ++i) {
        Token* currentToken = tokenSequence[i];
        if (currentToken != nullptr && currentToken->type == TokenType::ERROR) {
            outputFile << "ERROR" << " | " << currentToken->lexeme << " | " << currentToken->index << std::endl;
        }
    }
}