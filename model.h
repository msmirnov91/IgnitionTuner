#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "QString"
#include "QVector"
#include "QPointF"
#include <qwt_series_data.h>


class Model : public QObject
{
    Q_OBJECT

private:
    QVector<QPointF> data;

public:
    explicit Model(int, QObject *parent = 0);
    ~Model();

    double getTimingValue(int);
    double getRpmValue(int);
    int getAmountOfPoints();

signals:
    void modelUpdated(QVector<QPointF>);
    void dataChanged(QVector<QPointF>);

public slots:
    void setData(QVector<QPointF>);
    void setPoint(int, QPointF);

};

#endif // MODEL_H
