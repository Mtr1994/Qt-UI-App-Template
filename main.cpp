#include "mainwindow.h"
#include "Public/appconfig.h"

#include <QApplication>
#include <QFont>
#include <QFontMetricsF>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化配置文件
    AppConfig::getInstance()->init();

    // 写入基础尺寸
    float pointSize = QFontMetricsF(QFont("Microsoft Yahei", 9)).averageCharWidth();
    AppConfig::getInstance()->setValue("PointSize", "value", QString::number(pointSize * 2, 'f', 2));

    // 加载样式
    qApp->setStyleSheet("file:///:/Resource/qss/style.qss");

    MainWindow w;
    w.show();
    
    return a.exec();
}
