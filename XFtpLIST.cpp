#include "XFtpLIST.h"
#include "event2/bufferevent.h"
#include "event2/event.h"
#include "testUtil.h"
#include <string>
#include <sys/stat.h>


using namespace std;

 


void XFtpLIST::Write(bufferevent *bev) {

	testout("XFtpLIST::Write");
	ResCMD("226 Transfer comlete");
	ClosePORT();
}

void XFtpLIST::Event(bufferevent *bev, short events) {
	testout("At XFtpLIST::Event");
//    cout<< "cmdTask->listen_fd,"<< endl;
//    cout<< cmdTask->listen_fd << endl;
	if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
//    if (events & (BEV_EVENT_ERROR)) {
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the connection.\n",
                err, evutil_socket_error_to_string(err));
        // 处理错误，可能是关闭bufferevent，记录日志，报告给用户等。
//        bufferevent_free(bev);
//        cout<<"XFtpLIST::Event"<<endl;
		ClosePORT();
	}
	else if (events & BEV_EVENT_CONNECTED)
		cout << "XFtpLIST BEV_EVENT_CONNECTED" << endl;
}

//void timeout_cb(evutil_socket_t fd, short event, void *arg) {
//    printf("Timeout occurred!\n");
//    struct event_base *base = (struct event_base *)arg;
//    event_base_loopbreak(base);  // 停止事件循环
//}

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
    
		//  "-rwxrwxrwx 1 root root      418 Mar 21 16:10 XFtpFactory.cpp";
		string path = cmdTask->rootDir + cmdTask->curDir;
		testout("listpath: " << path);
		string listdata = GetListData(path);
		ConnectoPORT();   // 主动（命令）模式
//        cout<<"InitPASV()之前看看端口："<< cmdTask->port <<endl;
        
//        struct event *timeout_event = evtimer_new(cmdTask->base, timeout_cb, (void *)cmdTask->base);
//        struct timeval tv = {5, 0}; // 1 second
//        event_add(timeout_event, &tv);
//        InitPASV();    // 被动模式
//        if(bev){
//        cout<<"XFtpList::bev准备好了"<<endl;
//        ResCMD("150 Starting data transfer.");
        Send(listdata);
        ResCMD("150 Here coms the directory listing.");
//        }else{
//            cout<<"XFtpList::bev不存在"<<endl;
//        }
//        event_free(timeout_event);
	
		
	}
	else if (type == "CWD") //�л�Ŀ¼
	{
		//ȡ�������е�·��
		//CWD test\r\n
		int pos = msg.rfind(" ") + 1;
		//ȥ����β��\r\n
		string path = msg.substr(pos, msg.size() - pos - 2);
		string curDir = cmdTask->curDir;
		if (path[0] == '/') //�ֶ�·��
		{
			curDir = path;
		}
		else
		{
			if (curDir[curDir.size() - 1] != '/')
				curDir += "/";
			curDir += path + "/";
		}
		if (curDir[curDir.size() - 1] != '/')
			curDir += "/";
		struct stat s_buf;
		stat(curDir.c_str(),&s_buf);
		if(S_ISDIR(s_buf.st_mode))
		{
			cmdTask->curDir = curDir;
			ResCMD("250 Directory succes chanaged.\r\n");
		}
		else
		{
			ResCMD("501 Directory not chanaged.\r\n");
		}
		//cmdTask->curDir = curDir;
		//  /test/
		//ResCMD("250 Directory succes chanaged.\r\n");

		//cmdTask->curDir += 
	}
	else if (type == "CDUP") //�ص��ϲ�Ŀ¼
	{
		cout << "msg:" << msg << endl;
		cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
			string path = cmdTask->curDir;
			
		if (path[path.size() - 1] == '/')
		{
			path = path.substr(0, path.size() - 1);
		}
		int pos = path.rfind("/");
		path = path.substr(0, pos);
		cmdTask->curDir = path;
		if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/')
			cmdTask->curDir += "/";
		cout << "cmdTask->curDir:" << cmdTask->curDir << endl;
		ResCMD("250 Directory succes chanaged.\r\n");
	}
}

string XFtpLIST::GetListData(string path) {
	// -rwxrwxrwx 1 root root 418 Mar 21 16:10 XFtpFactory.cpp

	string data = "";
	string cmd = "ls -l ";
	cmd += path;
	cout << cmd << endl;
	FILE *f = popen(cmd.c_str(), "r");
	if (!f) return data;
	char buf[1024] = {0};
	while (1) {
		int len = fread(buf, 1, sizeof(buf) - 1, f);
		if (len <= 0)break;
		buf[len] = '\0';
		data += buf;
	}
	pclose(f);
 
	return data;
}
