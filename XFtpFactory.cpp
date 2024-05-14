#include "XFtpFactory.h"
#include "XFtpServerCMD.h"
#include "XFtpUSER.h"
#include "XFtpLIST.h"
#include "XFtpPORT.h"
#include "XFtpRETR.h"
#include "XFtpSTOR.h"
//#include "XFtpPASV.h"
#include "testUtil.h"
#include "XFtpDELE.h"
#include "XFtpRMD.h"
#include "XFtpSIZE.hpp"
// #include "XFtpDELE.h"
// 类定义外初始化
//bufferevent* XFtpTask::bevv = nullptr;

 
XTask *XFtpFactory::CreateTask() {
	testout("At XFtpFactory::CreateTask");
	XFtpServerCMD *x = new XFtpServerCMD();

	x->Reg("USER", new XFtpUSER());
//    x->Reg("PASV", new XFtpPASV());
	x->Reg("PORT", new XFtpPORT());

	XFtpTask *list = new XFtpLIST();
//    x->Reg("PASV", new XFtpPASV());
	x->Reg("PWD", list);
	x->Reg("LIST", list);     // LIST MLSD
	x->Reg("CWD", list);
	x->Reg("CDUP", list);

	x->Reg("RETR", new XFtpRETR());

	x->Reg("STOR", new XFtpSTOR());
	
    x->Reg("DELE", new XFtpDELE());
    x->Reg("RMD", new XFtpRMD());
    x->Reg("SIZE", new XFtpSIZE());

	return x;
}

XFtpFactory::XFtpFactory() {

}
