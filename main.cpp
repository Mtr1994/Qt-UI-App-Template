#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式
    qApp->setStyleSheet("file:///:/Resource/qss/style.qss");

    MainWindow w;
    w.show();
    
    // 处理因为 QSS 设置主窗口大小导致的程序默认不居中的问题
    w.move((a.primaryScreen()->availableSize().width() - w.width()) / 2, (a.primaryScreen()->availableSize().height() - w.height()) / 2);

    return a.exec();
}
