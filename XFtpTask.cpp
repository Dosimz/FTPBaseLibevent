#include "XFtpTask.h"
#include "testUtil.h"

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>

#include <string.h>
#include <iostream>
#include <sstream>

#include <netinet/in.h>
#include <sys/types.h>      // 基本系统数据类型
#include <sys/socket.h>     // 套接字基本函数
#include <netinet/in.h>     // Internet 地址族
#include <arpa/inet.h>      // IP 地址转换函数
#include <unistd.h>    // 对于 POSIX 系统调用 close()

#include <functional>       // std::bind

//#include <event2/event.h>


using namespace std;
// Detch
void XFtpTask::ConnectoPORT() {
	testout("At XFtpTask::ConnectoPORT");
	if (cmdTask->ip.empty() || cmdTask->port <= 0 || !cmdTask->base) {
		cout << "ConnectPORT failed" << endl;
		return;
	}
	if (bev) {
		bufferevent_free(bev);
		bev = 0;
	}
	bev = bufferevent_socket_new(cmdTask->base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(cmdTask->port);
    evutil_inet_pton(AF_INET, cmdTask->ip.c_str(), &sin.sin_addr.s_addr);

	
	Setcb(bev);

	timeval t = {60, 0};
	bufferevent_set_timeouts(bev, &t, 0);

	bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
}

void XFtpTask::ClosePORT() {
    cout<< "ClosePORT" << endl;
	if (bev) {
//        cout<< "bufferevent_free(bev);";
		bufferevent_free(bev);
		bev = 0;
	}

    
	if (fp) {
        cout<< "fclose(fp);";
		fclose(fp);
		fp = 0;
	}
}

void XFtpTask::InitPASV() {
    testout("At XFtpTask::InitPASV");
    //TODO
}

void XFtpTask::trimTrailingNewlines(std::string& str) {
    while (!str.empty() && (str.back() == '\n' || str.back() == '\r')) {
        str.pop_back(); // 删除字符串末尾的一个字符
    }
}


void XFtpTask::Setcb(bufferevent *nbev) {
	bufferevent_setcb(nbev, ReadCB, WriteCB, EventCB, this);
	bufferevent_enable(nbev, EV_READ | EV_WRITE);
}

void XFtpTask::Send(const string &data) {
	testout("At XFtpTask::Send");
	Send(data.c_str(), data.size());
}
void XFtpTask::Send(const char *data, size_t datasize) {
	testout("At XFtpTask::Send");
    cout << "快来看看这里面是什么啊" << endl;
	cout << data;
	cout << datasize << endl;
	if (datasize == 0) return;

	if (bev) {
        cout<<"bev is exits, now begin write"<<endl;
//        cout<<"我什么都不做会不会好一点"<<endl;
//        ::send(cmdTask->listen_fd, data, datasize, MSG_NOSIGNAL);
		bufferevent_write(bev, data, datasize);
    }else {
        cout<<" bev 好像还不存在";
    }
}

void XFtpTask::ResCMD(string msg) {
	testout("At XFtpTask::ResCMD");
	if (!cmdTask || !cmdTask->bev) return;
	cout << "ResCMD: " << msg << endl << flush;
	if (msg[msg.size() - 1] != '\n') {
		msg += "\r\n";
	}
	bufferevent_write(cmdTask->bev, msg.c_str(), msg.size());
}


void XFtpTask::EventCB(bufferevent *bev, short events, void *arg) {
//    cout << "EventCB被触发了" << endl;
	XFtpTask *t = (XFtpTask*)arg;
	t->Event(bev, events);
//    if (events & BEV_EVENT_CONNECTED) {
//        // 处理连接成功的事件，这对服务端来说可以是客户端成功连接
//        cout << "Connection accepted or connected successfully" << endl;
//    } else if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF)) {
//        // 处理错误或连接关闭
//        cout << "EventCB被触发了且错误或连接关闭" << endl;
//        t->ClosePORT();
//    }
}


//void XFtpTask::EventCB(bufferevent *bev, short events, void *arg) {
//    XFtpTask *t = (XFtpTask*)arg;
//    if (events & BEV_EVENT_CONNECTED) {
//        // 处理连接成功的事件，这对服务端来说可以是客户端成功连接
//        cout << "Connection accepted or connected successfully" << endl;
//    } else if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF)) {
//        // 处理错误或连接关闭
//        t->ClosePORT();
//    }
//    t->Event(bev, events);
//}

void XFtpTask::ReadCB(bufferevent *bev, void *arg) {
	XFtpTask *t = (XFtpTask*)arg;
	t->Read(bev);
}
void XFtpTask::WriteCB(bufferevent *bev, void *arg) {
//    cout<<"XFtpTask::WriteCB(bufferevent *bev, void *arg"<<endl;
	XFtpTask *t = (XFtpTask*)arg;
//    cout<<"XFtpTask *t = (XFtpTask*)arg;"<<endl;
	t->Write(bev);
}


XFtpTask::~XFtpTask() {
	ClosePORT();
}
