#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conf/appconfig.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("Mtr1994");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    float pointSize = stof(AppConfig::getInstance()->getValue("PointSize", "value"));
    setMinimumSize(pointSize * 100, pointSize * 100 * 0.618);

    // add user code here
}

