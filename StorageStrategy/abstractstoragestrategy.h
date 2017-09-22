#ifndef ABSTRACTSTORAGESTRATEGY_H
#define ABSTRACTSTORAGESTRATEGY_H

#include "model.h"
#include "QString"
#include "QPointF"
#include "QVector"

class AbstractStorageStrategy
{
protected:
    Model *model;
    QVector<QPointF> data;

    void clearData();

public:
    AbstractStorageStrategy(Model*);
    virtual void load(QString){}
    virtual void save(QString){}
};

#endif // ABSTRACTSTORAGESTRATEGY_H
