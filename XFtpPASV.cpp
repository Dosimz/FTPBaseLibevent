////
////  XFtpPASV.cpp
////  Xftp
////
////  Created by  鱼一一 on 2024/4/20.
////
//
//#include "XFtpPASV.h"
//#include "testUtil.h"
//#include <sstream>
//#include <iostream>
//#include <vector>
//#include "SyncResources.h"
//
//
//#include <netinet/in.h>
//#include <sys/types.h>      // 基本系统数据类型
//#include <sys/socket.h>     // 套接字基本函数
//#include <netinet/in.h>     // Internet 地址族
//#include <arpa/inet.h>      // IP 地址转换函数
//#include <unistd.h>    // 对于 POSIX 系统调用 close()
//
//#include <functional>       // std::bind
//
//#include <event2/bufferevent.h>
//#include <event2/event.h>
//#include <event2/buffer.h>
//#include <event2/listener.h>
//#include <event2/util.h>
//
//
//using namespace std;
//
//
//void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd,
//                    struct sockaddr *address, int socklen, void *ctx) {
//    cout<< "有连接进来了吧"<<endl;
//    XFtpTask* task = static_cast<XFtpTask*>(ctx);  // Cast ctx to XFtpTask*
////    struct event_base *base = evconnlistener_get_base(listener);
//    task->bev = bufferevent_socket_new(task->cmdTask->base, fd, BEV_OPT_CLOSE_ON_FREE);
//    
//    task->Setcb(task->bev);
//    timeval t = {60, 0};
//    bufferevent_set_timeouts(task->bev, &t, 0);
//    string ss = "slslslsllslslsls";
////    task->bufferevent_write(task->bev, ss, ss.size());
//    cout<<"尝试传输数据"<<endl;
//    task->ResCMD("数据连接想尝试传输数据");
////    task->Send(ss);
//    if (task) {
//        if(task->bev){
////            cout<<"nevv是什么结果啊:"<< nbev << endl;
//            XFtpTask::bevv = task->bev;
//            cout<<"XFtpTask::bevv是什么结果啊:"<< XFtpTask::bevv <<endl;
//        }
//        else{
//            cout<<"task->bevv = nbev;"<<endl;
//        }
//    }
////    cout<< "已经给 bevv 赋值"<<endl;
//}
//
//
//void XFtpPASV::Write(bufferevent *bev) {
//    cout<<"void XFtpPASV::Write(bufferevent *bev) {"<<endl;
//    testout("XFtpLIST::Write");
//    ResCMD("226 Transfer comlete");
//    ClosePORT();
//}
//
//
//void XFtpPASV::Event(bufferevent *bev, short events) {
//    cout<<("At XFtpPASV::Event,At XFtpPASV::Event,At XFtpPASV::Event")<<endl;
////    cout<< "cmdTask->listen_fd "<< endl;
////    cout<< cmdTask->listen_fd << endl;
//    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
////    if (events & (BEV_EVENT_ERROR)) {
//        int err = EVUTIL_SOCKET_ERROR();
//        fprintf(stderr, "Got an error %d (%s) on the connection.\n",
//                err, evutil_socket_error_to_string(err));
//        // 处理错误，可能是关闭bufferevent，记录日志，报告给用户等。
////        bufferevent_free(bev);
////        cout<<"XFtpLIST::Event"<<endl;
//        ClosePORT();
//    }
//    else if (events & BEV_EVENT_CONNECTED)
//        cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
//}
//
//
//
//void XFtpPASV::Parse(std::string type, std::string msg) {
//    if (type != "PASV") return;
//    
////    InitPASV();
//    
//    if(!XFtpTask::bevv){
//        // 选择一个动态端口进行监听
//        int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
//        sockaddr_in sin;
//        memset(&sin, 0, sizeof(sin));
//        sin.sin_family = AF_INET;
//        sin.sin_addr.s_addr = htonl(INADDR_ANY);
//    //    sin.sin_port = htons(0); // 系统自动分配s端口
//        cmdTask->port =  21211;
//        sin.sin_port = htons(cmdTask->port);
//        
//        if (::bind(listen_fd, (sockaddr *)&sin, sizeof(sin)) < 0) {
//            perror("Bind failed in PASV mode");
//            ::close(listen_fd);
//            return;
//        }
//        listen(listen_fd, 1);
//
//        socklen_t len = sizeof(sin);
//        if (getsockname(listen_fd, (sockaddr *)&sin, &len) == -1) {
//            perror("Getsockname failed in PASV mode");
//            ::close(listen_fd);
//    //        cout << "Getsockname failed in PASV mo" << endl;
//            return;
//        }
//    //    cmdTask->listen_fd = listen_fd;
//
//
//        testout("At XFtpTask::InitPASV");
//        if (!cmdTask->base) {
//            cout << "InitPASV failed: event base is null" << endl;
//            return;
//        }
//        if (bev) {
//            bufferevent_free(bev);
//            bev = nullptr;
//        }
//
//    //    struct evconnlistener *listener;
//    //    struct event_base *base0 = event_base_new();
//    //    listen_fd = cmdTask->listen_fd;
//        listener = evconnlistener_new(cmdTask->base, accept_conn_cb, this,    // cmdTask->base
//                                      LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1, listen_fd);
//
//        if (!listener) {
//            perror("Could not create a listener");
//            close(listen_fd);
//    //        event_base_free(base);
//            exit(1);
//        }
//        
//
//    //
//        pasvPort = cmdTask->port;
//        std::stringstream ss;
//
//        ip = "127.0.0.1";
//        
//        ss << "227 Entering Passive Mode (";
//        // 替换点为逗号，构造正确的IP地址格式
//        for (auto &c : ip) {
//            if (c == '.') {
//                ss << ',';
//            } else {
//                ss << c;
//            }
//        }
//        // 添加端口号，端口号为两部分，分别为高字节和低字节
//        ss << ',' << (pasvPort / 256) << ',' << (pasvPort % 256) << ").";
//        ResCMD(ss.str());
//    }
//    else{
//        ResCMD("已经有bevv了");
//    }
//    
//
////    ResCMD("等等，我这就去监听端口啊");
//    
////    // 设置超时
////    struct timeval t = {60, 0}; // 60秒超时
////    bufferevent_set_timeouts(bev, &t, NULL); // 只设置读超时
//}
//
//
//
//
////void XFtpPASV::Event(bufferevent *bev, short events) {
////    testout("At XFtpPASV::Event");
//////    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
////    if (events & (BEV_EVENT_ERROR)) {
////        int err = EVUTIL_SOCKET_ERROR();
////        fprintf(stderr, "Got an error %d (%s) on the connection.\n",
////                err, evutil_socket_error_to_string(err));
////        // 处理错误，可能是关闭bufferevent，记录日志，报告给用户等。
//////        bufferevent_free(bev);
////        cout<<"XFtpLIST::Event"<<endl;
////        ClosePORT();
////    }
////    else if (events & BEV_EVENT_CONNECTED)
////        cout << "XFtpPASV BEV_EVENT_CONNECTED" << endl;
////}
//// 确保监听套接字的生命周期和资源管理适当处理
