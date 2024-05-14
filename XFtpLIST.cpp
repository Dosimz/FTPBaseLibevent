#include "XFtpLIST.h"
#include "event2/bufferevent.h"
#include "event2/event.h"
#include "testUtil.h"
#include <string>
//#include <sys/stat.h>
#include <filesystem>
#include <iostream>
#include <cstdio>
#include <array>

using namespace std;

 


void XFtpLIST::Write(bufferevent *bev) {

	testout("XFtpLIST::Write");
	ResCMD("226 Transfer comlete");
	ClosePORT();
}

void XFtpLIST::Event(bufferevent *bev, short events) {
	testout("At XFtpLIST::Event");
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
//    if (events & (BEV_EVENT_ERROR)) {
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the connection.\n",
                err, evutil_socket_error_to_string(err));
 
		ClosePORT();
	}
	else if (events & BEV_EVENT_CONNECTED)
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
}
 

void XFtpLIST::Parse(std::string type, std::string msg) {
	testout("At XFtpLIST::Parse");
	string resmsg = "";
	if (type == "PWD") {
		// 257 "/" is current directory.
		resmsg = "257 \"";
		resmsg += cmdTask->curDir;
		resmsg += "\" is current dir.";
		ResCMD(resmsg);
	}  // LIST    MLSD   || type == "LIST"
	else if (type == "LIST") {
		string path = cmdTask->rootDir + cmdTask->curDir;
		testout("listpath: " << path);
		string listdata = GetListData(path);
		ConnectoPORT();   // 主动（命令）模式
        Send(listdata);
        ResCMD("150 Here coms the directory listing.");
 
	}
	else if (type == "CWD")
	{
		//CWD test\r\n
        std::string command = "CWD ";
        size_t pos = msg.find(command);
        if (pos == 0) {
            msg = msg.substr(command.length());
        }
        trimTrailingNewlines(msg);
//		string curDir = cmdTask->curDir;
//		if (msg[0] == '/')
//		{
//			curDir = msg;
//		}
//		else
//		{
//			if (curDir[curDir.size() - 1] != '/')
//				curDir += "/";
//			curDir += msg + "/";
//		}
//		if (curDir[curDir.size() - 1] != '/')
//			curDir += "/";
//		struct stat s_buf;
//		stat(curDir.c_str(),&s_buf);
        filesystem::path curDir = cmdTask->curDir;
        filesystem::path newPath = filesystem::path(msg).is_absolute() ? filesystem::path(msg) : curDir / filesystem::path(msg);
        newPath = filesystem::canonical(newPath); // 规范化路径，并处理相对路径
        
//		if(S_ISDIR(s_buf.st_mode))
        if (filesystem::is_directory(newPath))
		{
			cmdTask->curDir = newPath;
			ResCMD("250 Directory succes chanaged.\r\n");
		}
		else
		{
			ResCMD("501 Directory not chanaged.\r\n");
		}
	}
	else if (type == "CDUP")
	{
		cout << "msg:" << msg << endl;
		cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
//			string path = cmdTask->curDir;
        filesystem::path curDir = cmdTask->curDir;
//		if (path[path.size() - 1] == '/')
//		{
//			path = path.substr(0, path.size() - 1);
//		}
//		int pos = path.rfind("/");
//		path = path.substr(0, pos);
//		cmdTask->curDir = path;
//		if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
//			cmdTask->curDir += "/";
//		cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
        
        filesystem::path parentPath = curDir.parent_path(); // 直接获取父路径
        cmdTask->curDir = parentPath;
		ResCMD("250 Directory succes chanaged.\r\n");
	}
}




std::string XFtpLIST::GetListData(std::string path) {
    // 使用 std::array 替代原始的 char 数组
    std::array<char, 1024> buf;
    std::string data = "";

    // 防止命令注入，对 path 进行验证或清理
    // 这里假设 path 已经被验证过了，或者添加相应的验证代码

    // 使用更安全的方式构建命令字符串
    std::string cmd = "ls -l \"" + path + "\"";
    std::cout << cmd << std::endl;

    // 使用 std::unique_ptr 自动关闭 FILE 指针
    std::unique_ptr<FILE, decltype(&pclose)> f(popen(cmd.c_str(), "r"), pclose);
    if (!f) {
        std::cerr << "Failed to run command" << std::endl;
        return data;
    }

    // 循环读取数据，直到文件结束
    while (fgets(buf.data(), buf.size(), f.get()) != nullptr) {
        data += buf.data();
    }

    // 文件自动关闭由 unique_ptr 管理
    return data;
}


//string XFtpLIST::GetListData(string path) {
//	// -rwxrwxrwx 1 root root 418 Mar 21 16:10 XFtpFactory.cpp
//
//	string data = "";
//	string cmd = "ls -l ";
//	cmd += path;
//	cout << cmd << endl;
//	FILE *f = popen(cmd.c_str(), "r");
//	if (!f) return data;
//	char buf[1024] = {0};
//	while (1) {
//		int len = fread(buf, 1, sizeof(buf) - 1, f);
//		if (len <= 0)break;
//		buf[len] = '\0';
//		data += buf;
//	}
//	pclose(f);
// 
//	return data;
//}
