#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "log/applogger.h"
#include "public/threadsafequeue.h"

class UdpClientListener : public QObject, public CUdpClientListener
{
    Q_OBJECT
public:
    explicit UdpClientListener()
    {
        auto func = std::bind(&UdpClientListener::distributeData, this);
        std::thread th(func);
        th.detach();
    };

    ~UdpClientListener()
    {
        LOG_DEBUG("UdpClientListener release");
        mDistributeData = false;
    }

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnConnect(IUdpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        LOG_DEBUG("connect with server");
        //emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

    EnHandleResult OnHandShake(IUdpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        return HR_OK;
    }

    EnHandleResult OnSend(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        //QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        return HR_OK;
    }

    using CUdpClientListener::OnReceive;
    EnHandleResult OnReceive(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        //LOG_DEBUG("iLength = {}", iLength);
        mTotalSize += iLength;
        //std::string data = std::string((const char*)pData, iLength);
        std::lock_guard<std::mutex> lock(mMutexData);
        mDataQueue.push(std::string((const char*)pData, iLength));
        //mCvData.notify_one();
        //mBufferData.append(data);
        //emit AppSignal::getInstance()->sgl_recv_multibeam_data(data, iLength);
        //LOG_DEBUG("recv data size {}", iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(enOperation); Q_UNUSED(iErrorCode);Q_UNUSED(dwConnID);
        //emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        LOG_DEBUG("mTotalSize = {}", mTotalSize);
        return HR_OK;
    }

    void distributeData()
    {
        std::string buffer;
        while (mDistributeData)
        {
            std::unique_lock<std::mutex> lock(mMutexData);
            if (mDataQueue.empty())
            {
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            std::string pack = mDataQueue.pop();
            lock.unlock();
            buffer.append(pack);

            //LOG_DEBUG("buffer size {} {}", buffer.size(), mDataQueue.empty());

            while (buffer.size() > 4)
            {
                size_t pos = buffer.find("$IOA");
                if (pos == std::string::npos)
                {
                    buffer.clear();
                    break;
                }

                buffer = buffer.substr(pos);

                unsigned short id = 0;
                memcpy(&id, buffer.data() + 4, 2);

                uint32_t len = 0;
                memcpy(&len, buffer.data() + 12, 4);
                if (buffer.size() < len) break;

                std::string pack = buffer.substr(0, len);
                buffer = buffer.substr(len);

                emit AppSignal::getInstance()->sgl_recv_multibeam_data(pack, pack.length());
            }
        }
        LOG_DEBUG("distribute data {} closed.", mDataQueue.size());
    }

private:
    uint64_t    mTotalSize = 0;
    bool        mActiveStatus = true;
    bool        mDistributeData = true;

    std::mutex mMutexData;
    std::condition_variable mCvData;
    ThreadSafeQueue<std::string> mDataQueue;
};

class UdpSocket : public QObject
{
    Q_OBJECT
public:
    explicit UdpSocket(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start();
    void stop(int32_t dwConnID);
    void clear();
    void close();
    void edit(const QString &address, uint16_t port);
    void send(const std::string &data, uint32_t length, int32_t dwConnID);

private:
    QString mPeerAddress;
    uint16_t mPeerPort;

    UdpClientListener mUdpClientListener;
    CUdpClientPtr mUdpClientPtr;
};

#endif // UDPSOCKET_H
