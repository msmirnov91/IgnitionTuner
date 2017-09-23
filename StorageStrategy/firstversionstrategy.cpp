#include "firstversionstrategy.h"
#include <math.h>


FirstVersionStrategy::FirstVersionStrategy(Model *model) : 
                            AbstractStorageStrategy(model){
     hexFile = new IntelHex();
}


FirstVersionStrategy::~FirstVersionStrategy(){
    delete hexFile;
}


void FirstVersionStrategy::load(QString fileName){
    clearData();
    hexFile->parse(fileName);
    parseTable();
    data.pop_front(); // delete point (0, 0xff)
    data.pop_back(); // delete point (0xff, 0)
    model->setData(data);
}


void FirstVersionStrategy::parseTable(){
    setCursorToTableBeginning();

    int limiter = 0;

    // it is supposed that the last value corresponding to rpm form the table is 'FF'
    // and the table inevitably must contain that value
    while(true){
        int correspondingToRpmValue = readNextValueFromTable();
        int correspondingToTimingValue = readNextValueFromTable();

        QPointF newDataPoint = makeDataPoint(correspondingToRpmValue,
                                             correspondingToTimingValue);
        data.append(newDataPoint);

        limiter++;

        if (correspondingToRpmValue == 255){
            break;
        }

        if (limiter > 256){
            throw QString("Table does not have last point");
        }
    }
}


void FirstVersionStrategy::setCursorToTableBeginning(){
    currentStringIndex = 21;
    currentString = hexFile->getStringByIndex(currentStringIndex);
    cursorPosition = 22;
}


int FirstVersionStrategy::readNextValueFromTable(){
    QString textFromTable;
    int maximumCursorPosition = currentString.length();

    if (cursorPosition == maximumCursorPosition){ // we are at the end of string now
        getNextString();
        textFromTable = readByteFromCurrentString();
        skipByte();
    }
    else if (cursorPosition + 3 > maximumCursorPosition){// one part of value is in current string,
        textFromTable = readByteFromCurrentString(); // other part is in next string
        getNextString();
        skipByte();
    }
    else{ // just read the value
        textFromTable = readByteFromCurrentString();
        skipByte();
    }

    // FIXME: may be problems here
    bool ok;
    return textFromTable.toInt(&ok, 16);
}


void FirstVersionStrategy::getNextString(){
    currentStringIndex += 1;
    currentString = hexFile->getStringByIndex(currentStringIndex);
    cursorPosition = 2;
}


QString FirstVersionStrategy::readByteFromCurrentString(){
    QString result = currentString.mid(cursorPosition, 2);
    cursorPosition += 2;
    return result;
}


void FirstVersionStrategy::skipByte(){
    cursorPosition += 2;
}


QPointF FirstVersionStrategy::makeDataPoint(int valueForRpm, int valueForTiming){
    // Actually, valueForRpm, stored in the table, is amount of
    // impulses from Hall sensor occuring in some time period.
    // In my firmware that period is 0.2 sec. Izh Jupiter has two cylinders
    // working in antiphase. Thus, one revolution correspondes to two impulses,
    // and valueForRpm = ((n*2) / 60)*0.2, where n are RPM. The RPM value
    // can be expressed as valueForRpm*60/(2*0.2).

    float rpm = valueForRpm*60/(constants.impulsesPerRevolution*constants.measuringTime);

    // Timing value must be expressed in degrees. It means the diference between
    // the position of crankshaft when the spark must occur and the top dead point.
    // In the table value, corresponding to timtng(valueForTiming), expresses the amount
    // of timer ticks before the spark will be generated. Thus, to calculate the timing value
    // in degrees, we need to know the position of Hall sensor in degrees, current RPM value,
    // value from table and period of one timer tick (in seconds). The dependence
    // of those values can be defined by the expression:
    // timing = 360 - [hallPos + omega*t*valueForTiming], where
    // omega = rpm*360/60 (angular velocity in deg/sec),
    // t is one timer tick period in sec

    float omega = rpm*constants.rpmToOmegaCoeff;

    float timing = 360 - (constants.hallPos + omega*constants.tickPeriod*valueForTiming);

    return QPointF(rpm, timing);
}


void FirstVersionStrategy::save(QString fileName){
    setCursorToTableBeginning();

    writeNextPointToTable("0000FF00");

    for(int i = 0; i < model->getAmountOfPoints(); i++){
        QString pointForTable = makePointForTable(model->getRpmValue(i),
                                                     model->getTimingValue(i));
        writeNextPointToTable(pointForTable);
    }

    writeNextPointToTable("FF000000");
    hexFile->updateString(currentStringIndex, currentString);

    hexFile->save(fileName);
}


QString FirstVersionStrategy::makePointForTable(float rpm, float timing){
    // see QPointF makeDataPoint(float valueForRpm, float valueForTiming)
    // to find the explanation of formulas

    float floatValueForRpm = rpm*constants.impulsesPerRevolution*constants.measuringTime/60;
    unsigned int valueForRpm = convertToUint(floatValueForRpm);

    float omega = rpm*constants.rpmToOmegaCoeff;

    float floatValueForTiming = (360 - timing - constants.hallPos)/(omega*constants.tickPeriod);
    unsigned int valueForTiming = convertToUint(floatValueForTiming);

    QString textForRpm = QString("%1").arg(valueForRpm, 2, 16, QLatin1Char('0'));
    QString textForTiming = QString("%1").arg(valueForTiming, 2, 16, QLatin1Char('0'));
    QString result = textForRpm + "00" + textForTiming + "00";
    return result.toUpper();
}


unsigned int FirstVersionStrategy::convertToUint(float value){
    value = roundf(value);
    unsigned int result = (unsigned int) value;
    result &= 255;
    return result;
}


void FirstVersionStrategy::writeNextPointToTable(QString value){
    int maximumCursorPosition = currentString.length();
    int valueLength = value.length();

    if(cursorPosition == maximumCursorPosition){ // we are at the end of string now
        hexFile->updateString(currentStringIndex, currentString);
        getNextString();
        writeToCurrentString(value);
    }
    else if(cursorPosition + valueLength - 1 > maximumCursorPosition){   // one part of value is in current string,
                                                        // other part is in next string
        int toFirstStringLength = maximumCursorPosition - cursorPosition;
        int toNextStringLength = cursorPosition + valueLength - maximumCursorPosition;

        QString toFirstString = value.mid(0, toFirstStringLength);
        writeToCurrentString(toFirstString);
        hexFile->updateString(currentStringIndex, currentString);
        getNextString();

        QString toNextString = value.mid(toFirstStringLength, toNextStringLength);
        writeToCurrentString(toNextString);
    }
    else{ // just write the value
        writeToCurrentString(value);
    }
}


void FirstVersionStrategy::writeToCurrentString(QString value){
    int symbolsLength = value.length();
    currentString.replace(cursorPosition, symbolsLength, value);
    cursorPosition += symbolsLength;
}

