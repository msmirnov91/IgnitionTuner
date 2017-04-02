#ifndef VIEW_H
#define VIEW_H

#include <QObject>

#include "ui_mainwindow.h"
#include "qwt_plot_curve.h"
#include "qwt_series_data.h"


class View_wrong : public QObject
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QwtPlotCurve *mainCurve;

public:
    explicit View_wrong(Ui::MainWindow *ui, QObject *parent = 0);
    ~View_wrong();

signals:
    void loadData();

public slots:
    void plot(double *rpms, double *lags, int amountOfPoints);
    void tempSlot();

};

#endif // VIEW_H
