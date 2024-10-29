#ifndef APPSIGNAL_H
#define APPSIGNAL_H

#include <QObject>
#include <QRectF>

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

    void sgl_recv_multibeam_data(const std::string& data, uint64_t length);

    void sgl_recv_multibeam_bath_data(uint32_t number, float* points, const std::vector<double> &ranges);

};

#endif // APPSIGNAL_H
