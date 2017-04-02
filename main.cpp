#include "model.h"
#include "view.h"
#include "presenter.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Model *dataModel = new Model(12);
    View *uiView = new View();
    Presenter *presenter = new Presenter(dataModel, uiView);

    uiView->show();

    return a.exec();
}
