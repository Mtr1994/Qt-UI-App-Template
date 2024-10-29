#ifndef MULTIBEAMPARSER_H
#define MULTIBEAMPARSER_H

#include <QObject>
#include <mutex>
#include <queue>

class MultiBeamParser : public QObject
{
    Q_OBJECT
public:
    MultiBeamParser(QObject *parent = nullptr);
    ~MultiBeamParser();
    MultiBeamParser(const MultiBeamParser& signal) = delete;
    MultiBeamParser& operator=(const MultiBeamParser& signal) = delete;

public:
    static MultiBeamParser* getInstance();
    void init();

    void test1();

private:
    void parse();
    void parseMessage();
    void parsePack0xA1(const std::string& package, uint64_t length);
    void parsePack0xA2(const std::string& data, uint64_t length);
    void parsePack0xA3(const std::string& data, uint64_t length);
    void parsePack0xA4(const std::string& data, uint64_t length);
    void parsePack0xA5(const std::string& pack, uint64_t length);
    void parsePack0xA6(const std::string& data, uint64_t length);

public slots:
    void slot_recv_multibeam_data(const std::string& data, uint64_t length);

private:
    std::queue<std::string> mQueueMessage;
    std::mutex mMutexMessage;
    std::condition_variable mCVMessage;

    bool mInitStatus = false;
    bool mWorkingStatus = true;

    std::string mBufferString;

    uint64_t mTotalSize = 0;
};

#endif // MULTIBEAMPARSER_H
