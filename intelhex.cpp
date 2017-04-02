#include "intelhex.h"
#include "QFile"
#include "QTextStream"


IntelHex::IntelHex(){

}

void IntelHex::clear(){
    prefixes.clear();
    addresses.clear();
    strings.clear();
    checksums.clear();
}

void IntelHex::parse(QString fileName){
    clear();
    parseFile(fileName);
    if (!isValidFile()){
        throw QString("Hex file has bad format");
    }
}


void IntelHex::parseFile(QString fileName){
    QFile intelHexFile(fileName);
    if (intelHexFile.open(QIODevice::ReadOnly)){
        QTextStream in(&intelHexFile);
           while (!in.atEnd()){
              parseString(in.readLine());
           }
           intelHexFile.close();
    }
}


void IntelHex::parseString(QString string){
    if (isWellFormated(string) && checkSumIsCorrect(string)){
        // delete ':'
        string = string.mid(1);

        // two simbols for type, two for checksum, 4 for address
        int dataLength = string.length() - 8;

        prefixes.append(string.mid(0,2));
        addresses.append(string.mid(2,4));
        strings.append(string.mid(6, dataLength));
        checksums.append(string.mid(dataLength+6, 2));
    }
    else{
        throw QString("Bad string format or checksum is incorrect");
    }
}


bool IntelHex::isWellFormated(QString string){
    bool lengthIsOk = (string.length() >= 9);
    bool lengthIsOdd = (string.length()% 2 == 1);
    bool beginningIsOk = (string.mid(0,1) == ":");

    // mabe here i must ensure that
    //string does not contain bad simbols

    return lengthIsOk && lengthIsOdd && beginningIsOk;
}


bool IntelHex::checkSumIsCorrect(QString string){
    int checkSum = calculateByteSum(string.mid(1));
    return (checkSum == 0) ;
}


int IntelHex::calculateByteSum(QString string){
    int byteSum = 0;
    int currentIndex = 0;

    for (int i = 0; i < string.length(); i++ ){
        QString currentByte = string.mid(currentIndex, 2);

        bool ok;
        int currentByteValue = currentByte.toInt(&ok, 16);

        byteSum += currentByteValue;
        currentIndex += 2;
    }

    byteSum &= 255;
    return byteSum;
}


bool IntelHex::isValidFile(){
    int lastIndex = strings.length() - 1;
    QString endStringOfValidFile = ":00000001FF\r\n";
    QString lastString = makeString(lastIndex);

    bool result = (lastString == endStringOfValidFile);
    return result;
}


QString IntelHex::getStringByIndex(int index){
    if (index < strings.length()){
        return strings[index];
    }
    else{
        return "";
    }
}


void IntelHex::updateString(int index, QString newString){
    if (index < strings.length()){
        strings[index] = newString;
        updateChecksum(index);
    }
}


void IntelHex::updateChecksum(int index){
    QString newString = prefixes[index] + addresses[index] + strings[index];
    int byteSum = calculateByteSum(newString);

    int checkSum = 256 - byteSum;
    QString checkSumString = QString("%1").arg(checkSum, 0, 16);
    checkSumString = checkSumString.toUpper();
    checksums[index] = checkSumString;
}


void IntelHex::save(QString fileName){
    QFile intelHexFile(fileName);
    if (intelHexFile.open(QIODevice::WriteOnly)){
        QTextStream out(&intelHexFile);
        for (int i = 0; i < prefixes.length(); i++){
           QString string = makeString(i);
           out << string;
        }
        intelHexFile.close();
    }
}


QString IntelHex::makeString(int index){
    QString result =  ":" + prefixes[index] + addresses[index] +
                      strings[index] + checksums[index] + "\r\n";
    return result;
}


