#ifndef FIRSTVERSIONSTRATEGY_H
#define FIRSTVERSIONSTRATEGY_H

#include "abstractstoragestrategy.h"
#include "intelhex.h"

class FirstVersionStrategy : public AbstractStorageStrategy
{

struct FirmwareConstants{
    int impulsesPerRevolution;
    float measuringTime;

    float hallPos;
    float xtalFreq;
    int timerDivisor;
    float tickPeriod;

    float rpmToOmegaCoeff;

    FirmwareConstants(){
        impulsesPerRevolution = 2;
        measuringTime = 0.2; // in seconds

        hallPos = 340; // in degrees
        xtalFreq = 1.6e+7; // 16 MHz
        timerDivisor = 1024;
        tickPeriod = timerDivisor/xtalFreq; // in seconds

        rpmToOmegaCoeff = 360/60;
    }
};

private:
    FirmwareConstants constants;

    IntelHex *hexFile;

    QString currentString;
    int currentStringIndex;
    int cursorPosition;

    void setCursorToTableBeginning();

    void parseTable();

    int readNextValueFromTable();
    QString readByteFromCurrentString();
    void skipByte();
    void getNextString();
    QPointF makeDataPoint(int, int);

    QString makePointForTable(float, float);
    unsigned int convertToUint(float);
    void writeNextPointToTable(QString);
    void writeToCurrentString(QString);


public:
    FirstVersionStrategy(Model*);
    ~FirstVersionStrategy();

    virtual void load(QString);
    virtual void save(QString);
};

#endif // FIRSTVERSIONSTRATEGY_H
