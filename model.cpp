#include "model.h"
#include "QTextStream"
#include "QFile"


Model::Model(int amountOfPoints, QObject *parent) :
    QObject(parent){
}


Model::~Model(){
}


void Model::setData(QVector<QPointF> data){
    this->data = data;
    emit dataChanged(this->data);
}


double Model::getTimingValue(int index){
    QPointF neededPoint = data.at(index);
    return neededPoint.y();
}


double Model::getRpmValue(int index){
    QPointF neededPoint = data.at(index);
    return neededPoint.x();
}


int Model::getAmountOfPoints(){
    return data.size();
}


void Model::setPoint(int pointIndex, QPointF newPoint){
    data[pointIndex] = newPoint;
    emit modelUpdated(data);
}


