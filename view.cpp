#include "view.h"
#include "ui_view.h"
#include "QFileDialog"
#include "QDir"

View::View(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::View){
    ui->setupUi(this);
    ignitionTimingCurve = new QwtPlotCurve("Ignition Timing");
    ignitionTimingCurve->attach(ui->mainPlot);

    QObject::connect(this->ui->submitButton, SIGNAL(clicked()),
                     this, SLOT(submitButtonPressed()));
    QObject::connect(this->ui->listOfRpms, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(rpmsListIndexChanged(int)));
    QObject::connect(this->ui->initWithTestDataButton, SIGNAL(clicked()),
                     this, SLOT(testDataButtonPressed()));
    QObject::connect(this->ui->loadButton, SIGNAL(clicked()),
                     this, SLOT(chooseFileToLoadData()));
    QObject::connect(this->ui->saveButton, SIGNAL(clicked()),
                     this, SLOT(chooseFileToSaveData()));

}


View::~View(){
    delete ui;
    delete ignitionTimingCurve;
}


void View::submitButtonPressed(){
    int newPointIndex = ui->listOfRpms->currentIndex();

    double rpm = ui->listOfRpms->currentText().toDouble();
    double timing = ui->newValueLineEdit->text().toDouble();
    QPointF newPoint(rpm, timing);

    emit curvePointChanged(newPointIndex, newPoint);
}

void View::testDataButtonPressed(){
    emit initializeModelWithTestData();
}


void View::rpmsListIndexChanged(int newIndex){
    emit selectedPointIndex(newIndex);
}


void View::setCurrentTimingValue(QString value){
   ui->currentValueLineEdit->setText(value);
}


void View::plotCurve(QVector<QPointF> data){
    ignitionTimingCurve->setSamples(data);
    ui->mainPlot->replot();
}


void View::setRpmsList(QStringList rpms){
    clearRpmList();
    ui->listOfRpms->addItems(rpms);
}


void View::clearRpmList(){
    QObject::disconnect(this->ui->listOfRpms, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(rpmsListIndexChanged(int)));

    ui->listOfRpms->clear();

    QObject::connect(this->ui->listOfRpms, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(rpmsListIndexChanged(int)));
}


void View::chooseFileToLoadData(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Load data"),
                                QDir::currentPath(),
                                "Firmware (*.hex);;Source (*.asm);;All files (*.*)");
    if (fileName != ""){
        emit loadModelFromFile(fileName);
    }
}


void View::chooseFileToSaveData(){
    QString fileName = QFileDialog::getSaveFileName(this,
                                QString::fromUtf8("Save file"),
                                QDir::currentPath(),
                                "Firmware (*.hex);;Source (*.asm);;All files (*.*)");
    if (fileName != ""){
        emit saveModelToFile(fileName);
    }
}
