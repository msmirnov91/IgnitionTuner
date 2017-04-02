#include "presenter.h"

Presenter::Presenter(Model *model, View *view, QObject *parent) :
    QObject(parent){
    this->model = model;
    this->view = view;

    testDataStrategy = new TestDataStorageStrategy(this->model);
    firstVersionStrategy = new FirstVersionStrategy(this->model);

    // signals from view to presenter
    QObject::connect(this->view, SIGNAL(selectedPointIndex(int)),
                     this, SLOT(updateCurrentTimingValue(int)));
    QObject::connect(this->view, SIGNAL(initializeModelWithTestData()),
                     this, SLOT(loadTestData()));
    QObject::connect(this->view, SIGNAL(loadModelFromFile(QString)),
                     this, SLOT(loadData(QString)));
    QObject::connect(this->view, SIGNAL(saveModelToFile(QString)),
                     this, SLOT(saveToFile(QString)));

    // signals from model to presenter
    QObject::connect(this->model, SIGNAL(dataChanged(QVector<QPointF>)),
                     this, SLOT(initializeView(QVector<QPointF>)));
    QObject::connect(this->model, SIGNAL(modelUpdated(QVector<QPointF>)),
                     this, SLOT(updateCurve(QVector<QPointF>)));

    // signals from view to model
    QObject::connect(this->view, SIGNAL(curvePointChanged(int,QPointF)),
                     this->model, SLOT(setPoint(int,QPointF)));
}


Presenter::~Presenter(){
    delete testDataStrategy;
    delete firstVersionStrategy;
}


void Presenter::updateCurrentTimingValue(int newIndex){
    double timing = model->getTimingValue(newIndex);
    view->setCurrentTimingValue(doubleToQString(timing));
}


QString Presenter::doubleToQString(double number){
    return QString("%1").arg(number);
}


void Presenter::initializeView(QVector<QPointF> data){
    QStringList rpmsList;
    for (int i = 0; i < model->getAmountOfPoints(); i++){
        double rpm = model->getRpmValue(i);
        rpmsList << doubleToQString(rpm);
    }

    view->setRpmsList(rpmsList);
    updateCurve(data);
}


void Presenter::updateCurve(QVector<QPointF> data){
    view->plotCurve(data);
}


void Presenter::loadData(QString fileName){
    setStrategy("firstVersion");
    currentStrategy->load(fileName);
}


void Presenter::loadTestData(){
    setStrategy("testStrategy");
    currentStrategy->load("test");;
}


void Presenter::setStrategy(QString strategyName){
    if (strategyName == "testStrategy"){
        currentStrategy = testDataStrategy;
    }
    else if (strategyName == "firstVersion"){
        currentStrategy = firstVersionStrategy;
    }
    else{
        throw QString("Unknown strategy");
    }
}


void Presenter::saveToFile(QString fileName){
    currentStrategy->save(fileName);
}
