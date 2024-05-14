//
//  XFtpRMD.hpp
//  Xftp
//
//  Created by  鱼一一 on 2024/4/23.
//


#pragma once
#include "XFtpTask.h"
class XFtpRMD :
    public XFtpTask
{
public:
    virtual void Parse(std::string, std::string);
    bool removeDirectory(const std::string& path);
};

