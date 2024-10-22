#include "appconstants.h"
#include "log/applogger.h"

#include <QJsonObject>
#include <QJsonParseError>
#include <QListView>
#include <QFile>

QJsonDocument AppConstants::mJsonDocument;

AppConstants::AppConstants()
{

}

void AppConstants::init()
{
    // 填充状态码
    QFile file(":/resource/json/constants.json");
    if (!file.open(QFile::ReadOnly))
    {
        LOG_DEBUG("json config file open failed.");
        return;
    }
    mJsonDocument = QJsonDocument::fromJson(file.readAll());
    if (mJsonDocument.isEmpty()) LOG_DEBUG("json config file parse failed.");
}

void AppConstants::fillComboBox(ItemType type, QComboBox *cb)
{
    if (mJsonDocument.isEmpty())
    {
        init();
    }

    QJsonObject obj = mJsonDocument.object().value(QString::number(type)).toObject();
    QStringList keys = obj.keys();
    for (int i = 1; i <= keys.count(); i++)
    {
       int index = keys.indexOf(QString::number(i));
       if (index < 0) continue;
       cb->addItem(obj.value(keys.at(index)).toString(), keys.at(index).toInt());
    }
    cb->setView(new QListView());

    // 设置无阴影
    cb->view()->parentWidget()->setWindowFlags(Qt::NoDropShadowWindowHint | Qt::Popup | Qt::FramelessWindowHint);
    cb->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
}
