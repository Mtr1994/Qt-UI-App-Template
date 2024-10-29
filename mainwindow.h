#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network/udpsocket.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private slots:
    void slot_btn_connect_click();
    void slot_btn_test_click();

private:
    Ui::MainWindow *ui;

    UdpSocket mUdpSocket;
};
#endif // MAINWINDOW_H
