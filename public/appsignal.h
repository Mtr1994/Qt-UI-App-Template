﻿#ifndef APPSIGNAL_H
#define APPSIGNAL_H

#include <QObject>


class AppSignal : public QObject
{
    Q_OBJECT
private:
    explicit AppSignal(QObject *parent = nullptr);
    AppSignal(const AppSignal& signal) = delete;
    AppSignal& operator=(const AppSignal& signal) = delete;

public:
    static AppSignal* getInstance();

signals:
    void sgl_recv_system_message(const QString &msg, int status);

};

#endif // APPSIGNAL_H
