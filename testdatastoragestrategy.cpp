#include "testdatastoragestrategy.h"

TestDataStorageStrategy::TestDataStorageStrategy(Model *model):
                            AbstractStorageStrategy(model){
}


void TestDataStorageStrategy::load(QString fileName){
    for (int i = 0; i < 13; i++){
        double rpm = i*500;
        double timing = i;
        data.append(QPointF(rpm, timing));
    }
    model->setData(data);
}


void TestDataStorageStrategy::save(QString fileName){

}
