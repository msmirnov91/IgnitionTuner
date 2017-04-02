#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>

#include "model.h"
#include "view.h"
#include "abstractstoragestrategy.h"
#include "testdatastoragestrategy.h"
#include "firstversionstrategy.h"

class Presenter : public QObject
{
    Q_OBJECT
private:
    Model *model;
    View *view;

    AbstractStorageStrategy *currentStrategy;
    TestDataStorageStrategy *testDataStrategy;
    FirstVersionStrategy *firstVersionStrategy;

    QString doubleToQString(double);

public:
    explicit Presenter(Model*, View*, QObject *parent = 0);
    ~Presenter();

public slots:
    void updateCurrentTimingValue(int);
    void initializeView(QVector<QPointF>);
    void updateCurve(QVector<QPointF>);

    void setStrategy(QString);
    void loadData(QString);
    void loadTestData();
    void saveToFile(QString);
};

#endif // PRESENTER_H
