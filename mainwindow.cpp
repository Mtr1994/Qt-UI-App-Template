#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Public/appconfig.h"

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
    float pointSize = AppConfig::getInstance()->getValue("PointSize", "value").toFloat();
    setMinimumSize(pointSize * 124, pointSize * 124 * 0.618);

    // add user code here
}

