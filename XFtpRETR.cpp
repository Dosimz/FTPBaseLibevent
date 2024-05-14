#include "XFtpRETR.h"
#include "testUtil.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>
using namespace std;

// 定义一个全局的读写锁
std::shared_mutex rw_mutex;

void XFtpRETR::Parse(string type, string msg) {
	testout("At XFtpRETR::Parse");
	int pos = msg.rfind(" ") + 1;
	string filename = msg.substr(pos, msg.size() - pos - 2);
	string path = cmdTask->rootDir + cmdTask->curDir + filename;
	testout("filepath:[" << path << "]");
    rw_mutex.lock();
	fp = fopen(path.c_str(), "rb");
	if (fp) {
        rw_mutex.unlock();
		ConnectoPORT();
		ResCMD("150 File OK");
		bufferevent_trigger(bev, EV_WRITE, 0);
	}
	else {
        rw_mutex.unlock();
		ResCMD("450 file open failed!");
	}
}

void XFtpRETR::Write(bufferevent *bev) {
	testout("At XFtpRETR::Write");
	if (!fp) return;
	int len = fread(buf, 1, sizeof(buf), fp);
	if (len <= 0) {
		ResCMD("226 Transfer complete");
		ClosePORT();
		return;
	}
	cout << buf;
	Send(buf, len);   // sizeof(buf)
}

void XFtpRETR::Event(bufferevent *bev, short events) {
	testout("At XFtpRETR::Event");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
		ClosePORT();
	}
	else if (events & BEV_EVENT_CONNECTED) {
		cout << "TXFtpRETR BEV_EVENT_CONNECTED" << endl;
	}
}




