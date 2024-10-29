#include "udpsocket.h"

// test
#include <QDebug>

UdpSocket::UdpSocket(const QString &address, uint16_t port, QObject *parent)
    : QObject{parent}, mPeerAddress(address), mPeerPort(port)
{
    mUdpClientPtr.Attach(UdpClient_Creator::Create(&mUdpClientListener));
}

void UdpSocket::start()
{
    if (mUdpClientPtr->GetState() <= SS_STARTED) return;
    mUdpClientPtr->SetMaxDatagramSize(30720);
    mUdpClientPtr->Start(LPCTSTR(mPeerAddress.toStdString().data()), mPeerPort);
}

void UdpSocket::stop(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    mUdpClientPtr->Stop();
}

void UdpSocket::clear()
{
    // nothing to do here
}

void UdpSocket::close()
{
    mUdpClientListener.setActiveStatus(false);
    mUdpClientPtr->Stop();
    deleteLater();

    //emit sgl_delete_network_object_finish();
}

void UdpSocket::edit(const QString &address, uint16_t port)
{
    mPeerAddress = address;
    mPeerPort = port;
    //emit AppSignal::getInstance()->sgl_update_network_object(getObjectDetail(-1).token);
}

void UdpSocket::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    if (!mUdpClientPtr.IsValid()) return;
    mUdpClientPtr->Send((BYTE*)data.data(), (int)length);
}
