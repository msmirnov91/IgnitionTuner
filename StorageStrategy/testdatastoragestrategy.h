#ifndef TESTDATASTORAGESTRATEGY_H
#define TESTDATASTORAGESTRATEGY_H


#include "abstractstoragestrategy.h"

class TestDataStorageStrategy : public AbstractStorageStrategy
{
public:
    TestDataStorageStrategy(Model*);

    virtual void load(QString);
    virtual void save(QString);
};

#endif // TESTDATASTORAGESTRATEGY_H
