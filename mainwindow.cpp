﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("Mtr1994 可视化模板");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

}

