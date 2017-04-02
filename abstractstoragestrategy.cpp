#include "abstractstoragestrategy.h"

AbstractStorageStrategy::AbstractStorageStrategy(Model *model){
    this->model = model;
}

void AbstractStorageStrategy::clearData(){
    data.clear();
}
