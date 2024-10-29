#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocol/multibeamparser.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), mUdpSocket("192.168.2.108", 6000)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    setWindowTitle("Mtr1994");

    float dpi = screen()->physicalDotsPerInch();
    resize(dpi * 10, dpi * 10 * 0.618);

    // 初始化数据解析模块
    MultiBeamParser::getInstance()->init();

    // add user code here
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::slot_btn_connect_click);
    connect(ui->btnTest, &QPushButton::clicked, this, &MainWindow::slot_btn_test_click);

    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_multibeam_data, MultiBeamParser::getInstance(), &MultiBeamParser::slot_recv_multibeam_data);
}

void MainWindow::slot_btn_connect_click()
{
    mUdpSocket.start();
}

void MainWindow::slot_btn_test_click()
{
    MultiBeamParser::getInstance()->test1();
    ui->widgetMultiBeamView->test2();
}

