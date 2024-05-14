#pragma once
#include "XFtpTask.h"
#include <string>
using namespace std;

class XFtpPASV : public XFtpTask
{
public:
//    void Parse(std::string type, std::string msg) override;
    virtual void Parse(std::string, std::string);
    virtual void Event(bufferevent *, short);
    virtual void Write(bufferevent *);
    
//    void Init(int port, std::string ip);
//    bool Init() override;
private:
    int pasvPort; // 用于数据传输的监听端口
    std::string serverIp; // 服务器的IP地址};
};
