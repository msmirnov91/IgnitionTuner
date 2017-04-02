#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "qwt_plot_curve.h"

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

private:
    Ui::View *ui;
    QwtPlotCurve *ignitionTimingCurve;

    void clearRpmList();

public:
    explicit View(QWidget *parent = 0);
    ~View();

    void setCurrentTimingValue(QString);
    void plotCurve(QVector<QPointF>);
    void setRpmsList(QStringList);

private slots:
    void submitButtonPressed();
    void testDataButtonPressed();
    void rpmsListIndexChanged(int);

    void chooseFileToLoadData();
    void chooseFileToSaveData();


signals:
    void loadModelFromFile(QString);
    void saveModelToFile(QString);
    void initializeModelWithTestData();

    void curvePointChanged(int, QPointF);
    void selectedPointIndex(int);
};

#endif // VIEW_H
