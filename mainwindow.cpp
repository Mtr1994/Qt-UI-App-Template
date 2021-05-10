#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("木头人可视化模板");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QScreen *screen = QGuiApplication::screens().at(0);
    float width = 1024;
    float height = 640;
    if (nullptr != screen)
    {
        QRect rect = screen->availableGeometry();
        width = rect.width() * 0.1 < 1024 ? 1024 : rect.width() * 0.1;
        height = rect.width() * 0.1 < 640 ? 640 : rect.width() * 0.1;
    }

    resize(width, height);
}

