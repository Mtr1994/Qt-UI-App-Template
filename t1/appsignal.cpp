#include "appsignal.h"
#include <qmetatype.h>

AppSignal::AppSignal(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<uint16_t>("uint16_t");
}

AppSignal *AppSignal::getInstance()
{
    static AppSignal appSignal;
    return &appSignal;
}
