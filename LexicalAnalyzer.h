#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <string>
#include "TokenContainer.h"

class LexicalAnalyzer {
public:

    // ����������� ��������� ��� ����� ������: ���� ��� ������� ������, ������ ��� �������
    LexicalAnalyzer(const std::string& inputFileName, const std::string& outputFileName);

    // ����������, ����������� ����� � ������������� �������
    ~LexicalAnalyzer();
    void analyze();

private:
    std::ifstream inputFile;
    std::ofstream outputFile;
    TokenList tokenList;


    // ������ ��� ������������� ������

    // ����������� ���� �������������� (�������� ����� ��� ��� ����������)
    TokenType classifyIdentifier(const std::string& lexeme);

    // ����������� ���� ����� (����� �����)
    TokenType classifyNumber(const std::string& lexeme);

    // ��������, �������� �� ������ ����������
    bool checkIfOperator(const std::string& str) const;

    // ��������, �������� �� ������ ������������
    bool checkIfDelimiter(char c) const;

    // ��������� ������, ���� �� �������� ����������
    void processOperatorToken(char c);

    // ��������� ������, ���� �� �������� ������������
    void processDelimiterToken(char c);

    // ���������� ���������� ������ �� �������� �����
    Token fetchNextToken();
};

#endif