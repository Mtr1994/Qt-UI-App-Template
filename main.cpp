#include "mainwindow.h"
#include "log/logger.h"
#include "conf/appconfig.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // init log
    Logger::init(Logger::LOG_CONSOLE);

    // 初始化配置文件
    AppConfig::getInstance()->init();

    // 加载样式
    qApp->setStyleSheet("file:///:/resource/qss/style.qss");

    MainWindow w;
    w.show();
    
    return a.exec();
}
