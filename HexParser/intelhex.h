#ifndef INTELHEXPARSER_H
#define INTELHEXPARSER_H

#include "QString"
#include "QVector"


class IntelHex
{
    QVector<QString> prefixes;
    QVector<QString> addresses;
    QVector<QString> strings;
    QVector<QString> checksums;

    void clear();

    void parseFile(QString);
    void parseString(QString);

    bool isValidFile();
    bool isValidString(QString);

    bool isWellFormated(QString);
    bool checkSumIsCorrect(QString);

    int calculateByteSum(QString);
    void updateChecksum(int);

    QString makeString(int);

public:
    IntelHex();

    void parse(QString);
    void save(QString);

    QString getStringByIndex(int);

    void updateString(int, QString);

};

#endif // INTELHEXPARSER_H
