#include "multibeamparser.h"
#include "log/applogger.h"
#include "public/appsignal.h"
#include "utils/geographichelper.h"

#include <QDateTime>
#include <thread>
#include <QRectF>

MultiBeamParser::MultiBeamParser(QObject *parent)
    : QObject{parent}
{

}

MultiBeamParser::~MultiBeamParser()
{
    mWorkingStatus = false;
    mCVMessage.notify_one();
}

MultiBeamParser *MultiBeamParser::getInstance()
{
    static MultiBeamParser inst;
    return &inst;
}

void MultiBeamParser::init()
{
    auto func = std::bind(&MultiBeamParser::parseMessage, this);
    std::thread th(func);
    th.detach();
    mInitStatus = true;
}

void MultiBeamParser::test1()
{
    LOG_DEBUG("MultiBeamParser mTotalSize = {}", mTotalSize);
}

void MultiBeamParser::parse()
{

}

void MultiBeamParser::parseMessage()
{
    while (mWorkingStatus)
    {
        std::unique_lock<std::mutex> lock(mMutexMessage);
        mCVMessage.wait(lock, [this]{ return !mQueueMessage.empty() || !mWorkingStatus; });

        if (!mWorkingStatus) break;

        std::string pacakge = mQueueMessage.front();
        mQueueMessage.pop();

        unsigned short id = 0;
        memcpy((char *)&id, pacakge.data() + 4, 2);
        switch (id) {
        case 0xA1:
            parsePack0xA1(pacakge, pacakge.size());
            break;
        case 0xA2:
            parsePack0xA2(pacakge, pacakge.size());
            break;
        case 0xA3:
            parsePack0xA3(pacakge, pacakge.size());
            break;
        case 0xA4:
            parsePack0xA4(pacakge, pacakge.size());
            break;
        case 0xA5:
            parsePack0xA5(pacakge, pacakge.size());
            break;
        case 0xA6:
            parsePack0xA6(pacakge, pacakge.size());
            break;
        default:
            LOG_DEBUG("unrecognized packet, abandon id = {}", id);
            break;
        }
    }
    LOG_DEBUG("message parser thread finished");
}

void MultiBeamParser::parsePack0xA1(const std::string &package, uint64_t length)
{
    LOG_DEBUG("parse A1 package");
    char mask[5] = {'\n'};
    memcpy((char *)&mask, package.data(), 4);
    if (std::string(mask) != "$IOA")
    {
        printf("An unknown error has occurred while reading file \n");
        return;
    }
    unsigned short id = 0;
    memcpy((char*)&id, package.data() + 4, 2);
    if (id != 0x00A1)
    {
        LOG_DEBUG("somethine is wrong here");
        return;
    }

    uint32_t len = 0;
    memcpy((char*)&len, package.data() + 12, 4);
    if (len != length)
    {
        LOG_DEBUG("different length here");
        return;
    }

    uint32_t nt = 0;
    memcpy(&nt, package.data() + 20,  4);

    uint32_t nr = 0;
    memcpy(&nr, package.data() + 24,  4);

    float *points = new float[nr * 4];
    uint32_t counter = 0;
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double maxZ = -std::numeric_limits<double>::max();

    for (unsigned int i = 0; i < nr; i++)
    {
        uint32_t index = 56 + nt * 40 + 60 * i;
        int32_t twoWayTravelTime = 0;
        memcpy(&twoWayTravelTime, package.data() + index + 12, 4);
        if (twoWayTravelTime == 0) continue;

        int32_t amp = 0;
        memcpy(&amp, package.data() + index + 16,  4);
        int32_t longitude = 0;
        memcpy(&longitude, package.data() + index + 24,  4);

        int32_t latitude = 0;
        memcpy(&latitude, package.data() + index + 28,  4);

        int32_t waterDepth = 0;
        memcpy(&waterDepth, package.data() + index + 32,  4);

        // 转换为墨卡托投影坐标
        double x, y;
        GeographicHelper::latLonToMercator(latitude * 0.0000001, longitude * 0.0000001, x, y);

        points[counter * 4 + 0] = x;
        points[counter * 4 + 1] = y;
        double z = waterDepth * -0.0001;
        points[counter * 4 + 2] = z;
        points[counter * 4 + 3] = amp * 0.0001;
        counter++;

        //LOG_DEBUG("x = {} y = {}", x, y);


        minX = qMin(minX, x); maxX = qMax(maxX, x);
        minY = qMin(minY, y); maxY = qMax(maxY, y);
        minZ = qMin(minZ, z); maxZ = qMax(maxZ, z);
    }

    QRectF rect = {minX, minY, maxX - minX, maxY - minY};
    emit AppSignal::getInstance()->sgl_recv_multibeam_bath_data(counter, points, rect, minZ, maxZ);
}

void MultiBeamParser::parsePack0xA2(const std::string &data, uint64_t length)
{
    //LOG_DEBUG("parse A2 package");
}

void MultiBeamParser::parsePack0xA3(const std::string &data, uint64_t length)
{
    //LOG_DEBUG("parse A3 package");
}

void MultiBeamParser::parsePack0xA4(const std::string &data, uint64_t length)
{
    //LOG_DEBUG("parse A4 package");
}

void MultiBeamParser::parsePack0xA5(const std::string &pack, uint64_t length)
{
    return;
    LOG_DEBUG("parse A5 package");

    uint64_t ping = 0;
    memcpy(&ping, pack.data() + 16, 4);
    LOG_DEBUG("ping = {}", ping);

    uint32_t date = 0;
    memcpy(&date, pack.data() + 28, 4);

    int year = date / 10000;
    int month = date % 1000 / 100;
    int day = date % 100;

    uint32_t millisec = 0;
    memcpy(&millisec, pack.data() + 32, 4);
    QDateTime datetime = QDateTime(QDate(year, month, day), QTime::fromMSecsSinceStartOfDay(millisec));

    //LOG_DEBUG("time is {}", datetime.toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString());
}

void MultiBeamParser::parsePack0xA6(const std::string &data, uint64_t length)
{
    //LOG_DEBUG("parse A6 package");
}

void MultiBeamParser::slot_recv_multibeam_data(const std::string &data, uint64_t length)
{
    mTotalSize += length;
    if (!mInitStatus)
    {
        LOG_DEBUG("please init message parser first");
        return;
    }
    std::lock_guard<std::mutex> lock(mMutexMessage);
    mQueueMessage.push(data);
    mCVMessage.notify_one();
}
