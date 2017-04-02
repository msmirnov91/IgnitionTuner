#include "view_wrong.h"

View_wrong::View_wrong(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent){
    this->ui = ui;
    mainCurve = new QwtPlotCurve("Lags");
}


View_wrong::~View_wrong(){
    delete mainCurve;
}


void View_wrong::tempSlot(){
    emit loadData();
}


void View_wrong::plot(double *rpms, double *lags, int amountOfPoints){
    mainCurve->setSamples(rpms, lags, amountOfPoints);
    mainCurve->attach(ui->mainPlot);
    ui->mainPlot->replot();
}
